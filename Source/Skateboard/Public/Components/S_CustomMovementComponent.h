// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "S_CustomMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKATEBOARD_API US_CustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

#pragma region SpeedProperties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Speed")
	float MaxSkateSpeed = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Speed")
	float ImpulseSpeed = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Speed")
	float DecelerationSpeed = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Speed")
	float ImpulseDecelerationSpeed = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Speed")
	float HorizontalDecelerationSpeed = 0.75;

#pragma endregion SpeedProperties

#pragma region RotationProperties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Rotation")
	float FloorRotationControl = 0.75;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Rotation")
	float AirRotationControl = 0.1;

#pragma endregion RotationProperties

protected:

	virtual void BeginPlay() override;

public:

	void SetMaxWalkSpeed(float MaxSpeed);
};
