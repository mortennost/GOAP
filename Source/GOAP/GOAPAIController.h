// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "GOAPState.h"
#include "GOAPStateEditor.h"
#include "GOAPSettings.h"
#include "GOAPPlanner.h"
#include "GOAPAtomKey.h"
#include "GOAPEQSJob.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GOAPAIController.generated.h"

UCLASS()
class GOAP_API AGOAPAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UGOAPPlanner* Planner;

	bool LoadGOAPDefaults();


protected:
	// Pointer to BBData asset to be spawned as component for this agent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	UBlackboardData* BlackboardDataAsset;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentPlanString;


public:
	// Constructor (used to initialize CrowdFollowingComponent (Detour behavior) as the default PathFollowingComponent)
	AGOAPAIController(const FObjectInitializer& objInitializer);

	UGOAPSettings* Settings = GetMutableDefault<UGOAPSettings>();


	// The currently active Plan (queue of actions)
	TQueue<TWeakObjectPtr<UGOAPAction>> ActionQueue;

	// The currently active Action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TWeakObjectPtr<UGOAPAction> CurrentAction;

	// Current Goal being pursued
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP")
	FGOAPAtom CurrentGoal;

	// Current WorldState of this agent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP")
	FGOAPState WorldState;

	// Available actions for this agent (runtime)
	UPROPERTY()
	TArray<UGOAPAction*> GOAPActions;
	

	// Queue for managing EQS requests from actions (Mpsc: Multiple producers, single consumer queue)
	TQueue<FGOAPEQSJob, EQueueMode::Mpsc> EQSJobsQueue;

	// Current EQS job being processed
	FGOAPEQSJob EQSCurrentJob;

	FEnvQueryRequest EQSRequest;
	

	// GOAP actions that will be available to this agent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	TArray<TSubclassOf<UGOAPAction>> AvailableActions;

	// Default starting state of the agent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	FGOAPStateEditor StartingState;

	// Default starting goal of the agent
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	FGOAPAtom DefaultGoal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	TArray<FGOAPGoal> Goals;

	// Maximum number of nodes able to be searched through in the planning graph. Prevents crash if graph gets stuck in a loop
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP")
	int32 MaxGraphNodes = 256;

	
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	// Possess is called when the character is spawned
	virtual void Possess(APawn* pawn) override;

	// Callback for when a MoveTo call has completed
	virtual void OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result) override;


	// ---- Helpers for changing to MoveTo states ----

	// Move to a Location
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetMoveToStateWithLocation(const FVector targetLocation, const float walkSpeed = 400.0f, const float acceptanceRadius = 50.0f);

	// Move to an Actor
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetMoveToStateWithActor(AActor* targetActor, const float walkSpeed = 400.0f, const float acceptanceRadius = 50.0f);

	// ---------


	// ---- Planner-related functionality ----

	// Stop the current action and plan in progress. Commonly used when sensor stimulus causes agent to need to replan
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void ClearCurrentActionAndPlan();

	// Activate the Planner to build a plan to satisfy the current goal
	bool BuildActionPlanForCurrentGoal();

	// Helper function for the planner, fetch the valid actions for a given state
	TArray<UGOAPAction*> GetValidActionsForState(const FGOAPState inState);

	// Helper to check if a given state is satisfied by the agent's current WorldState
	bool IsStateSatisfied(const FGOAPAtom inStateAtom);

	// Helper to check if a given state is satisfied by the agent's current WorldState
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool IsStateSatisfied(FGOAPAtomKey key, const bool value);
	
	// ---------
	

	// Get the string name of the action that is currently in progress
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	FString GetCurrentActionString();

	// Sets the state of a single atom, creates the entry if it doesn't already exist
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetWorldState(FGOAPAtomKey key, bool value);

	// Sets the state of a single atom, creates the entry if it doesn't already exist 
	//		(Utility function for Blueprints)
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetWorldStateWithAtom(FGOAPAtom atom, bool value);

	// Gets the state value of a single atom
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool GetWorldState(FGOAPAtomKey key);

	// Set the currently active goal
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void SetCurrentGoal(FGOAPAtomKey key, bool value);

	// Check if the provided goal is the one currently set
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool IsGoalSet(FGOAPAtomKey key, bool value);


	// Adds an EQS query job
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	void AddEQSJob(UGOAPAction* callingAction, UEnvQuery* query, TEnumAsByte<EEnvQueryRunMode::Type> runMode);

	// Called when an EQS query finishes
	void OnEQSQueryFinished(TSharedPtr<FEnvQueryResult> result);

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool IsMoveInProgress() const;


	// ---- Utility ----

	// Get Player Pawn reference
	UFUNCTION(BlueprintPure, Category = "Utility|GOAP")
	APawn* GetPlayerPawn();

	UFUNCTION(BlueprintPure, Category = "Utility|GOAP")
	static FVector InvalidLocation() { return FVector(FLT_MAX); }

	UFUNCTION(BlueprintPure, Category = "Utility|GOAP")
	static bool IsValidLocation(const FVector location) { return location != FVector(FLT_MAX) && location != FVector::ZeroVector; }

	UFUNCTION(BlueprintPure, Category = "Utility|GOAP")
	UBlackboardComponent* GetBlackboard() const { return Blackboard; }

	UFUNCTION(BlueprintCallable, Category = "Utility|GOAP")
	void GetDebugInfo(FString& currentGoalInfo, FString& currentPlanInfo) const;

	// ---------
};
