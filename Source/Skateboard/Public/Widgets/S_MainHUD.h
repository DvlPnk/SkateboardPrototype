// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "S_MainHUD.generated.h"

class ASkateboardCharacter;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SKATEBOARD_API US_MainHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ASkateboardCharacter* Player;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SpeedTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScoreTextBlock;

	UPROPERTY(EditAnywhere)
	int CurrentScore;

	UPROPERTY(EditAnywhere)
	int ScoreMultiplier = 10;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void AddScore(float Value);
};
