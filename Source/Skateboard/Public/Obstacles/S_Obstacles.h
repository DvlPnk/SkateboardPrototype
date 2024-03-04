// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S_Obstacles.generated.h"

class UBoxComponent;

UCLASS()
class SKATEBOARD_API AS_Obstacles : public AActor
{
	GENERATED_BODY()

protected:

#pragma region VisibleComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USceneComponent* PivotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObstacleMeshComponent;

	/** Collision created just to get reference of the height*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ObstacleCollisionComponent;

#pragma endregion

#pragma region Variables

	UPROPERTY(EditDefaultsOnly)
	float ObstacleHeight;

#pragma endregion

public:	
	// Sets default values for this actor's properties
	AS_Obstacles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	float GetObstacleHeight() const { return ObstacleHeight; }

};
