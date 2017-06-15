// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GOAPGameMode.generated.h"

UCLASS()
class GOAP_API AGOAPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGOAPGameMode();


protected:
	UPROPERTY()
	UUserWidget* AIDebugWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GOAP UI Debug")
	TSubclassOf<UUserWidget> AIDebugWidgetRef;


	virtual void BeginPlay() override;
};

