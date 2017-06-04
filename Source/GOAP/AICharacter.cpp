// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAP.h"
#include "AICharacter.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Make sure rotation is controlled by movement, not from yaw input
	UCharacterMovementComponent* movement = GetCharacterMovement();
	movement->bOrientRotationToMovement = true;
	movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

