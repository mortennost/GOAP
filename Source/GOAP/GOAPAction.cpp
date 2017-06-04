// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP.h"
#include "GOAPAIController.h"
#include "GOAPAction.h"

UGOAPAction::UGOAPAction(const class FObjectInitializer& objInitializer) : Super(objInitializer)
{

}

void UGOAPAction::SetupDefaults()
{
	for (FGOAPAtom& pre : Preconditions.State)
	{
		Preconditions_Internal.SetState(pre.Key, pre.Value);
	}

	for (FGOAPAtom& e : Effects.State)
	{
		Effects_Internal.SetState(e.Key, e.Value);
	}
	
	// Add some randomness to the tick rate to avoid evaluating on the same tick
	TimeSinceLastTick = FMath::FRandRange(0.0f, 1.0f);
	TimeSinceLastCostUpdate = FMath::FRandRange(0.0f, 1.0f);
}

bool UGOAPAction::ArePreconditionsSatisfied(AGOAPAIController* controller)
{
	if (Preconditions_Internal.State.Num() == 0)
	{
		return true;
	}

	return controller->WorldState.IsGOAPStateSatisfied(Preconditions_Internal);
}

bool UGOAPAction::IsInRange(AGOAPAIController* controller)
{
	if (ActionTarget == nullptr)
	{
		return false;
	}

	FVector actorLoc = controller->GetPawn()->GetActorLocation();
	FVector targetLoc = ActionTarget->GetActorLocation();

	float rangeToTarget = (actorLoc - targetLoc).Size();

	return rangeToTarget < InteractionRange;
}

bool UGOAPAction::AreEffectsSatisifed(AGOAPAIController* controller)
{
	return controller->WorldState.IsGOAPStateSatisfied(Effects_Internal);
}

AActor* UGOAPAction::GetBestActorResult()
{
	if (QueryResultsActor.Num() <= 0)
	{
		return nullptr;
	}
	
	return QueryResultsActor[0];
}

FVector UGOAPAction::GetBestLocationResult()
{
	if (QueryResultsLocation.Num() <= 0)
	{
		return FVector(FLT_MAX);
	}

	return QueryResultsLocation[0];
}
