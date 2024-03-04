// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/S_Obstacles.h"

#include "Components/BoxComponent.h"

// Sets default values
AS_Obstacles::AS_Obstacles()
{
	/** Creating Pivot and setting as Root*/
	PivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot Component"));
	RootComponent = PivotComponent;

	/** Creating Obstacle Mesh Component and attaching to root*/
	ObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Obstacle Mesh Component"));
	ObstacleMeshComponent->SetupAttachment(PivotComponent);
	ObstacleMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No; // Setting character can't step on

	/** Creating Obstacle Collision and attaching to root*/
	ObstacleCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Obstacle Collision Component"));
	ObstacleCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ObstacleMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No; // Setting character can't step on
}

// Called when the game starts or when spawned
void AS_Obstacles::BeginPlay()
{
	Super::BeginPlay();

	ObstacleHeight = ObstacleCollisionComponent->GetUnscaledBoxExtent().Z;
	ObstacleHeight *= GetActorScale().Z;
}