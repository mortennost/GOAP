// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "SmartObjectActor.generated.h"

UCLASS()
class GOAP_API ASmartObjectActor : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartObjectActor();

	// Set Team ID (Defaults to 255 == NoTeam/Neutral).
	FGenericTeamId TeamId = FGenericTeamId::NoTeam;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when a SmartObject is used
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP|SmartObject")
	void Use();
	virtual void Use_Implementation() {};
	
};
