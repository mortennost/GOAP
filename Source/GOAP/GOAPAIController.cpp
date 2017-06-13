// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP.h"
#include "GOAPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

AGOAPAIController::AGOAPAIController(const FObjectInitializer& objInitializer) :
	Super(objInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// Set TeamID. Default TeamID for AI agents are 1 (Player == 0)
	AAIController::SetGenericTeamId(FGenericTeamId(1));

	// Create Blackboard component
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("WS_BlackboardComponent"));
}

bool AGOAPAIController::LoadGOAPDefaults()
{
	CurrentGoal = DefaultGoal;

	// Create the actions
	for (TSubclassOf<UGOAPAction> action : AvailableActions)
	{
		GOAPActions.Add(NewObject<UGOAPAction>(this, action));
	}

	for (auto& action : GOAPActions)
	{
		action->SetupDefaults();
	}

	// Load the default state
	for (FGOAPAtom& state : StartingState.State)
	{
		WorldState.SetState(state.Key, state.Value);
	}

	if (BlackboardDataAsset != nullptr && Blackboard != nullptr)
	{
		bool bbInitialized = Blackboard->InitializeBlackboard(*BlackboardDataAsset);
		if (!bbInitialized)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Blackboard initialization failed!!"));
			return false;
		}

		Blackboard->SetValueAsVector("TargetLocation", InvalidLocation());
	}

	return true;
}

void AGOAPAIController::BeginPlay()
{
	Super::BeginPlay();

	Planner = NewObject<UGOAPPlanner>(this);
	
	LoadGOAPDefaults();
}

void AGOAPAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// If we don't have a current action waiting/running..
	if (CurrentAction == nullptr)
	{
		// If nothing is queued up, we need to try to form a new plan
		if (ActionQueue.IsEmpty())
		{
			// Try to form a plan
			if (BuildActionPlanForCurrentGoal())
			{
				// If the goal is already satisfied by the current WorldState, we'll try another one next tick
				if (IsStateSatisfied(CurrentGoal))
				{
// 					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("State already satisfied!!"));
					ActionQueue.Empty();
					return;
				}

				// Select the front action in the queue
				ActionQueue.Dequeue(CurrentAction);
			}
		}
		else // Queue not empty; select the next action in the queue
		{
			ActionQueue.Dequeue(CurrentAction);
		}

		if(CurrentAction != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Orange, TEXT("CurrentAction: ") + CurrentAction->ActionDescription);
		}
	}

	// CurrentAction should be set at this point. Check validity just to be sure.
	if (CurrentAction != nullptr && CurrentAction->IsValidLowLevel())
	{
		// If preconditions for current action aren't met, the plan is invalid, "complete" action and clear plan queue
		if (!CurrentAction->ArePreconditionsSatisfied(this) && CurrentAction->bAbortIfPreconditionsNotSatisfied)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CurrentAction no longer valid. Re-plan!!"));

			CurrentAction->Abort(this);
			ClearCurrentActionAndPlan();
		}
		else if (CurrentAction->AreEffectsSatisifed(this) && CurrentAction->bAbortIfEffectsAreSatisfied)
		{
			// If the target state is satisfied by the WorldState, just complete the action without clearing plan queue

			CurrentAction->Abort(this);
			CurrentAction = nullptr;
		}
		else if(CurrentAction->bShouldAbort)
		{
			// Important that this is reset. Maybe make OnReset() function??
			CurrentAction->bShouldAbort = false; 

			CurrentAction = nullptr;
		}
		else
		{
			// Otherwise, we just execute the action as normal
			//	 Though, we still need to adhere to the TickRate;

			CurrentAction->TimeSinceLastTick += deltaTime;

			if (CurrentAction->TimeSinceLastTick > CurrentAction->TickRate)
			{
				CurrentAction->TimeSinceLastTick = 0.0f;

				// Execute/update the action. If it returns true, it's complete
				if (CurrentAction->Execute(this, deltaTime))
				{
					// Apply effects of action to Agent's current WorldState
					if(CurrentAction->bApplyEffectsWhenFinished)
					{
						for (FGOAPAtom& eff : CurrentAction->Effects.State)
						{
							SetWorldState(eff.Key, eff.Value);
						}
					}

					// Clear the action ptr so we can try to process the next one in the queue on next tick
					CurrentAction = nullptr;
				}
			}
		}
	}

	// Check for action cost update ticks
	for (auto& action : GOAPActions)
	{
		action->TimeSinceLastCostUpdate += deltaTime;

		if (action->TimeSinceLastCostUpdate > action->CostUpdateRate)
		{
			action->TimeSinceLastCostUpdate = 0.0f;
			action->UpdateCost(this);
		}
	}

	// Process EQS Jobs if we don't already have an active Job
	if (!EQSCurrentJob.CallingAction.IsValid())
	{
		// Get the new/waiting Job (if one exists) and execute it.. 
		if (EQSJobsQueue.Dequeue(EQSCurrentJob))
		{
			EQSCurrentJob.CallingAction->bIsQueryResultsPending = true;
			EQSRequest = FEnvQueryRequest(EQSCurrentJob.Query, GetCharacter());
			EQSRequest.Execute(EQSCurrentJob.QueryRunMode, this, &AGOAPAIController::OnEQSQueryFinished);
		}
	}
}

void AGOAPAIController::Possess(APawn* pawn)
{
	Super::Possess(pawn);
}

void AGOAPAIController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
	Super::OnMoveCompleted(requestID, result);
}

void AGOAPAIController::SetMoveToStateWithLocation(const FVector targetLocation, const float walkSpeed, const float acceptanceRadius)
{
	if (targetLocation == InvalidLocation())
		return;

	DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), targetLocation, FColor::Blue, true, 1.0f, 0, 12.0f);

	// Set move speed
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Set in motion pathfinding to location
	MoveToLocation(targetLocation, acceptanceRadius);
}

void AGOAPAIController::SetMoveToStateWithActor(AActor* targetActor, const float walkSpeed, const float acceptanceRadius)
{
	if (targetActor == nullptr)
		return;

	DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), targetActor->GetActorLocation(), FColor::Blue, true, 1.0f, 0, 12.0f);

	// Set move speed
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Set in motion pathfinding to location
	MoveToActor(targetActor, acceptanceRadius);
}

void AGOAPAIController::ClearCurrentActionAndPlan()
{
	// Clearing the action and queue will cause IdleState to form a new plan
	CurrentAction = nullptr;
	ActionQueue.Empty();
}

bool AGOAPAIController::BuildActionPlanForCurrentGoal()
{
// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Building plan"));

	TArray<TWeakObjectPtr<UGOAPAction>> planActions;

	bool hasValidGoal = false;
	FGOAPGoal selectedGoal;

	// Find highest priority goal
	for (FGOAPGoal goal : Goals)
	{
		// The goal is already satisfied, discard it
		if (IsStateSatisfied(goal.GoalState))
			continue;

		if (goal.Priority > selectedGoal.Priority || !hasValidGoal)
		{
			selectedGoal = goal;
			hasValidGoal = true;
		}
	}

	if(!hasValidGoal)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("No valid goals found!"));
		return false;
	}

	CurrentGoal = selectedGoal.GoalState;

	// TODO: We only support single state goals right now... !!!
	FGOAPAtom targetState = selectedGoal.GoalState;

	// Call planner to make a plan
	planActions = Planner->Plan(this, MaxGraphNodes, targetState.Key, targetState.Value, &GOAPActions, &WorldState, *this);

	// If we find a plan, push it onto the ActionQueue
	if (planActions.Num() > 0)
	{
		for (int i = 0; i < planActions.Num(); i++)
		{
			ActionQueue.Enqueue(planActions[i]);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Could not find a plan!!"));
		return false;
	}

	return true;
}

TArray<UGOAPAction*> AGOAPAIController::GetValidActionsForState(const FGOAPState inState)
{
	TArray<UGOAPAction*> result;

	for (auto& action : GOAPActions)
	{
		if (action->Preconditions_Internal.IsGOAPStateSatisfied(inState))
		{
			result.Push(action);
		}
	}

	return result;
}

bool AGOAPAIController::IsStateSatisfied(const FGOAPAtom inStateAtom)
{
	return WorldState.IsStateSatisfied(inStateAtom.Key, inStateAtom.Value);
}

bool AGOAPAIController::IsStateSatisfied(FGOAPAtomKey key, const bool value)
{
	return WorldState.IsStateSatisfied(key.Key, value);
}

FString AGOAPAIController::GetCurrentActionString()
{
	if (CurrentAction != nullptr)
	{
		return CurrentAction->ActionDescription;
	}
	else
	{
		return TEXT("None");
	}
}

void AGOAPAIController::SetWorldState(FGOAPAtomKey key, bool value)
{
	WorldState.SetState(key.Key, value);
}

void AGOAPAIController::SetWorldStateWithAtom(FGOAPAtom atom, bool value)
{
	WorldState.SetState(FGOAPAtomKey(atom.Key).Key, value);
}

bool AGOAPAIController::GetWorldState(FGOAPAtomKey key)
{
	if (WorldState.State.Contains(key.Key))
	{
		return *WorldState.State.Find(key.Key);
	}
	else
	{
		// This is not ideal, but will do for now
		return false;
	}
}

void AGOAPAIController::SetCurrentGoal(FGOAPAtomKey key, bool value)
{
	CurrentGoal.Key = key.Key;
	CurrentGoal.Value = value;
	ClearCurrentActionAndPlan();
}

bool AGOAPAIController::IsGoalSet(FGOAPAtomKey key, bool value)
{
	if (CurrentGoal.Key == key.Key && CurrentGoal.Value == value)
	{
		return true;
	}

	return false;
}

void AGOAPAIController::AddEQSJob(UGOAPAction* callingAction, UEnvQuery* query, TEnumAsByte<EEnvQueryRunMode::Type> runMode)
{
	FGOAPEQSJob job;

	job.CallingAction = callingAction;
	job.Query = query;
	job.QueryRunMode = runMode;

	EQSJobsQueue.Enqueue(job);
}

void AGOAPAIController::OnEQSQueryFinished(TSharedPtr<FEnvQueryResult> result)
{
	if (EQSCurrentJob.CallingAction.IsValid())
	{
		EQSCurrentJob.CallingAction->QueryResultsActor.Empty();
		EQSCurrentJob.CallingAction->QueryResultsLocation.Empty();

		if (EQSCurrentJob.QueryRunMode == EEnvQueryRunMode::SingleResult)
		{
			// Add result (single) for this query to the list of results
			EQSCurrentJob.CallingAction->QueryResultsActor.Add(result->GetItemAsActor(0));
			EQSCurrentJob.CallingAction->QueryResultsLocation.Add(result->GetItemAsLocation(0));
		}
		else
		{
			// Add results (multiple) for this query to the list of results
			result->GetAllAsActors(EQSCurrentJob.CallingAction->QueryResultsActor);
			result->GetAllAsLocations(EQSCurrentJob.CallingAction->QueryResultsLocation);
		}

		EQSCurrentJob.CallingAction->bIsQueryResultsAvailable = true;
		EQSCurrentJob.CallingAction->bIsQueryResultsPending = false;
		EQSCurrentJob.CallingAction = nullptr;
		EQSRequest = nullptr;
	}
}

bool AGOAPAIController::IsMoveInProgress() const
{
	return GetMoveStatus() != EPathFollowingStatus::Idle;
}

APawn* AGOAPAIController::GetPlayerPawn()
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}
