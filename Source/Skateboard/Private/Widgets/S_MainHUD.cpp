// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/S_MainHUD.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Skateboard/SkateboardCharacter.h"

void US_MainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ASkateboardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(Player)
	{
		Player->OnAirTimeDelegate.BindUObject(this, &US_MainHUD::AddScore);
	}

}

void US_MainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(Player)
	{
		FString CurrentSpeed = FromIntToString(Player->GetMaxWalkSpeed(), 3);

		SpeedTextBlock->SetText(FText::FromString(CurrentSpeed));
	}

	UpdateScore();

}

void US_MainHUD::AddScore(float Value)
{
	CurrentScore += Value * ScoreMultiplier;
	bCanUpdateScore = true;
	ScoreUpdateTime = (float) 0.5 / (CurrentScore - LastScore);
}

void US_MainHUD::UpdateScore()
{
	if (LastScore == CurrentScore || !bCanUpdateScore)
	{
		return;
	}

	bCanUpdateScore = false;

	GetWorld()->GetTimerManager().SetTimer(ScoreUpdateTimerHandle, this,
		&US_MainHUD::SetCanUpdateScore, ScoreUpdateTime, false);

	LastScore += 1;

	FString ScoreString = FromIntToString(LastScore, 4);

	ScoreTextBlock->SetText(FText::FromString(ScoreString));
}

FString US_MainHUD::FromIntToString(int Value, int ZeroQuantity)
{
	FString FinalString = "";
	for(int i = 0; i < ZeroQuantity; i++)
	{
		FinalString.Append("0");
	}

	int TestValue = Value;

	while (TestValue / 10 > 0)
	{
		FinalString.RemoveAt(0);
		TestValue /= 10;
	}

	return FinalString.Append(FString::FromInt(Value));
}
