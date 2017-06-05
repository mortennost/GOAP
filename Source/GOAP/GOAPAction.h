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

UCLASS(Blueprintable, BlueprintType, meta = (ShowWorldContextPin))
class GOAP_API UGOAPAction : public UObject
{
	GENERATED_BODY()

private:
	
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

	// Pointer to the target Actor of this action (optional)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	TWeakObjectPtr<AActor> ActionTarget = nullptr;

	// Position that the agent needs to be at to perform this action (only used when state "AtLocation" is a precondition or effect)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	FVector AtLocationTarget = FVector(FLT_MAX);
	
	// Required range from target to perform this action
	//	 If outside this range, agent will move towards it using pathfinding
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	float InteractionRange;

	// If this action is active, should sensor stimuli interrupt MoveTo state
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GOAP Action")
	bool ShouldInterruptMoveOnTargetAcquisition = false;


	// Helpers for the use of EQS queries

	// Is there a pending query request?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	bool IsEQSQueryRequestPending;

	// Is there any EQS results available
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	bool IsEQSResultsAvailable;

	// Get the query results for actors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP Action")
	TArray<AActor*> QueryResultsActor;

	// Get the query results for locations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GOAP Action")
	TArray<FVector> QueryResultsLocation;


	// Configures the action with default settings, copying data to Internal
	void SetupDefaults();

	// Checks if the preconditions of this action are satisfied by this agent's current world state
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool ArePreconditionsSatisfied(AGOAPAIController* controller);

	// Checks if agent is within range of whatever it needs to be in range of
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool IsInRange(AGOAPAIController* controller);

	// Checks if this action's target state (effect) is already satisfied by this agent's current world state
	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	bool AreEffectsSatisifed(AGOAPAIController* controller);

	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	AActor* GetBestActorResult();

	UFUNCTION(BlueprintPure, Category = "GOAP|Actions")
	FVector GetBestLocationResult();

	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	bool CheckProceduralPreconditions(AGOAPAIController* controller);
	virtual bool CheckProceduralPreconditions_Implementation(AGOAPAIController* controller) { return true; }

	// Called each tick while this is the active action, returns true when completed
	//	 Can/should be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	bool Execute(AGOAPAIController* controller, float DeltaSeconds);
	virtual bool Execute_Implementation(AGOAPAIController* controller, float DeltaSeconds) { return false; };

	// Called each tick while this is the active action, returns true when completed
	//	 Can/should be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|Actions")
	void UpdateCost(AGOAPAIController* controller);
	virtual void UpdateCost_Implementation(AGOAPAIController* controller) {};
};
