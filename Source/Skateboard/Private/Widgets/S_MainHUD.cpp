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
		int MaxWalkSpeed = Player->GetMaxWalkSpeed();
		FString CurrentSpeed = "000";
		while(MaxWalkSpeed / 10 > 0)
		{
			CurrentSpeed.RemoveAt(0);
			MaxWalkSpeed /= 10;
		}

		CurrentSpeed.Append(FString::FromInt(Player->GetMaxWalkSpeed()));

		SpeedTextBlock->SetText(FText::FromString(CurrentSpeed));
	}
}

void US_MainHUD::AddScore(float Value)
{
	CurrentScore += Value * ScoreMultiplier;

	UE_LOG(LogTemp, Warning, TEXT("%i"), CurrentScore);

	int Score = CurrentScore;

	FString ScoreString = "0000";
	while (Score / 10 > 0)
	{
		ScoreString.RemoveAt(0);
		Score /= 10;
	}

	ScoreString.Append(FString::FromInt(CurrentScore));

	ScoreTextBlock->SetText(FText::FromString(ScoreString));
}
