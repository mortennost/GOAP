// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP.h"
#include "GOAPAIController.h"
#include "GOAPAction_GoToLocation.h"


bool UGOAPAction_GoToLocation::Execute_Implementation(AGOAPAIController* controller, float deltaSeconds)
{
	FVector targetLocation = controller->GetBlackboard()->GetValueAsVector(FName("TargetLocation"));

	if(targetLocation == AGOAPAIController::InvalidLocation())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TargetLocation value is invalid!!"));
		return false;
	}

	if(!bHasCalledMoveTo)
	{
		controller->SetMoveToStateWithLocation(targetLocation);
		bHasCalledMoveTo = true;
	}
	else
	{
		// No longer moving, meaning the MoveTo operation completed
		if(controller->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			controller->GetBlackboard()->SetValueAsVector(FName("CurrentLocation"), targetLocation);
			controller->GetBlackboard()->SetValueAsVector(FName("TargetLocation"), AGOAPAIController::InvalidLocation());

			return true;
		}
	}

	return false;
}
