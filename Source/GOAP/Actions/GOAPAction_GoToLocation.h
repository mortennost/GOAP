// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOAPAction.h"
#include "GOAPAction_GoToLocation.generated.h"

/**
 * 
 */
UCLASS()
class GOAP_API UGOAPAction_GoToLocation : public UGOAPAction
{
	GENERATED_BODY()
	
private:
	bool bHasCalledMoveTo;
	
public:
	// Position that the agent needs to be at to perform this action (only used when state "AtLocation" is a precondition or effect)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP Action")
	FVector LocationTarget = FVector(FLT_MAX);
	
	virtual bool Execute_Implementation(AGOAPAIController* controller, float deltaSeconds) override;
};
