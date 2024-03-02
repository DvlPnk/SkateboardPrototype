// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateboardGameMode.h"
#include "SkateboardCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkateboardGameMode::ASkateboardGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
