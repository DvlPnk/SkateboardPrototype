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

private:

	UPROPERTY(EditAnywhere)
	ASkateboardCharacter* Player;

#pragma region UIComponents

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SpeedTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScoreTextBlock;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AddingScoreTextBlock;*/

#pragma endregion

#pragma region ScoreVariables

	UPROPERTY()
	FTimerHandle ScoreUpdateTimerHandle;

	UPROPERTY()
	float ScoreUpdateTime = 0.05;

	UPROPERTY()
	bool bCanUpdateScore = true;

	UPROPERTY(EditAnywhere)
	int CurrentScore;

	UPROPERTY(EditAnywhere)
	int LastScore;

	UPROPERTY(EditAnywhere)
	int ScoreMultiplier = 12.8;

#pragma endregion

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void AddScore(float Value);

	UFUNCTION()
	void UpdateScore();

	UFUNCTION()
	void SetCanUpdateScore() { bCanUpdateScore = true; }
};
