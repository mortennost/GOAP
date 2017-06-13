// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOAP.h"
#include "GOAPAtom.h"
#include "GOAPState.h"
#include "GOAPStateEditor.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h"
#include "GOAPAction.generated.h"

class AGOAPAIController;

USTRUCT(BlueprintType)
struct FGOAPGoal
{
	GENERATED_BODY()

public:
	FGOAPGoal() {}
	~FGOAPGoal() {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Goal")
	FString GoalDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Goal")
	uint8 Priority;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Goal")
	FGOAPAtom GoalState;
};

UCLASS(Blueprintable, BlueprintType, meta = (ShowWorldContextPin))
class GOAP_API UGOAPAction : public UObject
{
	GENERATED_BODY()
		
public:
	UGOAPAction(const class FObjectInitializer& objInitializer);
	
	// Text string describing the action (for debugging)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	FString ActionDescription;

	// Preconditions (set of States) that must be satisfied for this to be a valid action
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	FGOAPStateEditor Preconditions;
	// Effects (set of States) applied to agent's current world state if this action is successfully completed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	FGOAPStateEditor Effects;

	// Internal state of this action's preconditions
	FGOAPState Preconditions_Internal;
	// Internal state of this action's effects
	FGOAPState Effects_Internal;

	// If true, all states in Effects will be automatically applied to the agent's WorldState upon successful execution.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	bool bApplyEffectsWhenFinished;
	// If true, this action will abort immediately if Preconditions are no longer satisfied during execution.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	bool bAbortIfPreconditionsNotSatisfied = true;
	// If true, this action will abort immediately if Effects are satisfied during execution.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	bool bAbortIfEffectsAreSatisfied = true;

	// How often this action should Execute/tick while active, default 0 = every tick
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	float TickRate;
	// Time since last Execute/tick
	float TimeSinceLastTick = 0.0f;

	// The cost of this action, used in planning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	int32 Cost;
	// Frequency to call this action's UpdateCost function
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	float CostUpdateRate = 60.0f;
	// Time since last UpdateCost
	float TimeSinceLastCostUpdate = 0.0f;
	
	// Required range from target to perform this action
	//	 If outside this range, agent will move towards it using pathfinding
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	float InteractionRange;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> QueryResultsActor;
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> QueryResultsLocation;
	UPROPERTY(BlueprintReadWrite)
	bool bIsQueryResultsPending;
	UPROPERTY(BlueprintReadWrite)
	bool bIsQueryResultsAvailable;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldAbort;

	
	// Configures the action with default settings, copying data to Internal
	void SetupDefaults();

	// Checks if the preconditions of this action are satisfied by this agent's current world state
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool ArePreconditionsSatisfied(AGOAPAIController* controller);

	// Checks if this action's target state (effect) is already satisfied by this agent's current world state
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool AreEffectsSatisifed(AGOAPAIController* controller);

	// TODO: Not yet implemented!
	// Checks if agent is within range of whatever it needs to be in range of
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool IsInRange(AGOAPAIController* controller);


	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	bool CheckProceduralPreconditions(AGOAPAIController* controller);
	virtual bool CheckProceduralPreconditions_Implementation(AGOAPAIController* controller) { return true; }

	// Called each tick while this is the active action, returns true when completed
	//	 Can/should be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	bool Execute(AGOAPAIController* controller, float DeltaSeconds);
	virtual bool Execute_Implementation(AGOAPAIController* controller, float deltaSeconds) { return false; };

	// Called each tick while this is the active action, returns true when completed
	//	 Can/should be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	void UpdateCost(AGOAPAIController* controller);
	virtual void UpdateCost_Implementation(AGOAPAIController* controller) {};

	// Called when action is running and Preconditions are no longer satisfied (only if 'bInterruptOnPreconditionsNotSatisfied == TRUE')
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP|Actions")
	void Abort(AGOAPAIController* controller);
	virtual void Abort_Implementation(AGOAPAIController* controller) { bShouldAbort = true; }


	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	AActor* GetQueryResultsActor(const uint8 index) const { return QueryResultsActor[index]; }

	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	FVector GetQueryResultsLocation(const uint8 index) const { return QueryResultsLocation[index]; }
};
