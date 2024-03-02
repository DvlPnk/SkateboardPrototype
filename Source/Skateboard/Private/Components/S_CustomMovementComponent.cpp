// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/S_CustomMovementComponent.h"

void US_CustomMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	JumpZVelocity = 200.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 600.f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
}

void US_CustomMovementComponent::SetMaxWalkSpeed(float MaxSpeed)
{
	MaxWalkSpeed = MaxSpeed;
}
