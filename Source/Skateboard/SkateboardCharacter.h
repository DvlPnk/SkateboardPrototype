// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "SkateboardCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class US_CustomMovementComponent;

UCLASS(config=Game)
class ASkateboardCharacter : public ACharacter
{
	GENERATED_BODY()

	// All Character Visible Components (SpringArm, Camera, StaticMeshes, etc)
#pragma region VisibleComponents

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SkateMeshComponent;

#pragma endregion

protected:

	// Input Mapping & Input Actions
#pragma region MappingContext

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReleaseForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReleaseHorizontalAction;

#pragma endregion

#pragma region SkateVariables

#pragma region ImpulseMove

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CurrentDecelerationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsTryingToImpulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsImpulsing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bAbleToImpulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsDecelerating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsAbleToDecelerate;

#pragma endregion

#pragma region HorizontalMove

	UPROPERTY(EditDefaultsOnly)
	FTimeline StabilizeTimeLine;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* StabilizeCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CurrentSkateRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CurrentHorizontalRotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsMovingHorizontally;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsAbleToMoveHorizontally;

#pragma endregion

#pragma region Jump

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	FTimerHandle JumpingTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsJumping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsAbleToJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float InAirTime;

#pragma endregion

#pragma endregion

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	US_CustomMovementComponent* CustomMovementComponent;

#pragma region Animation

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* PlayerAnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* PushMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* JumpMontage;

#pragma endregion Animation

#pragma region Sounds

	UPROPERTY(EditAnywhere)
	USoundBase* SkatePushSoundCue;

#pragma endregion Sounds

public:

	ASkateboardCharacter(const FObjectInitializer& ObjectInitializer);
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void StopForwardMovement(const FInputActionValue& Value);

	void StopHorizontalMovement(const FInputActionValue& Value);

	void InitJump();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns CustomMovementComponent **/
	FORCEINLINE US_CustomMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }

	float NormalizeValue(float Source, float MinVal, float MaxVal, float Multiplier = 1, bool bAffectsMultiplier = false);

	UFUNCTION(BlueprintCallable)
	void SpeedUpSkateboard();

	UFUNCTION(BlueprintCallable)
	void PlaySpeedUpSound();

	UFUNCTION(BlueprintCallable)
	void Jumping();

	UFUNCTION(BlueprintCallable)
	void RestoreSkateStates();

	UFUNCTION(BlueprintCallable)
	void ResetJump();

	UFUNCTION()
	void SetAbleToJump();

	UFUNCTION()
	void StabilizeCenterOfMass(float Value);
};

