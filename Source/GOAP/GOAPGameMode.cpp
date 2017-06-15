// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GOAP.h"
#include "GOAPGameMode.h"
#include "GOAPCharacter.h"

AGOAPGameMode::AGOAPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGOAPGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(AIDebugWidgetRef != nullptr)
	{
		AIDebugWidget = CreateWidget<UUserWidget>(GetWorld(), AIDebugWidgetRef);
		AIDebugWidget->AddToViewport();
	}
}
