// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Navigation/CrowdAgentInterface.h"
#include "GOAPCharacter.generated.h"

UCLASS(config=Game)
class AGOAPCharacter : public ACharacter, public IGenericTeamAgentInterface, public ICrowdAgentInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Team ID
	FGenericTeamId TeamId;

	// -- ICrowdAgentInterface implementation
//	virtual FVector GetCrowdAgentLocation() const override;
//	virtual FVector GetCrowdAgentVelocity() const override;
//	virtual void GetCrowdAgentCollisions(float& cylinderRadius, float& cylinderHalfHeigh) const override;
//	virtual float GetCrowdAgentMaxSpeed() const override;
//	virtual int32 GetCrowdAgentAvoidanceGroup() const override;
//	virtual int32 GetCrowdAgentGroupsToAvoid() const override;
//	virtual int32 GetCrowdAgentGroupsToIgnore() const override;


public:
	AGOAPCharacter();

//	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
//	float BaseTurnRate;
//
//	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
//	float BaseLookUpRate;

protected:

//	/** Resets HMD orientation in VR. */
//	void OnResetVR();
//
//	/** Called for forwards/backward input */
//	void MoveForward(float Value);
//
//	/** Called for side to side input */
//	void MoveRight(float Value);
//
//	/** 
//	 * Called via input to turn at a given rate. 
//	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
//	 */
//	void TurnAtRate(float Rate);
//
//	/**
//	 * Called via input to turn look up/down at a given rate. 
//	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
//	 */
//	void LookUpAtRate(float Rate);
//
//	/** Handler for when a touch input begins. */
//	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
//
//	/** Handler for when a touch input stops. */
//	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

