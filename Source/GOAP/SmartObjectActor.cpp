// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP.h"
#include "SmartObjectActor.h"


// Sets default values
ASmartObjectActor::ASmartObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ASmartObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmartObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

