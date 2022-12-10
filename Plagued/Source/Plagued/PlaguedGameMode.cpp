// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlaguedGameMode.h"
#include "PlaguedCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlaguedGameMode::APlaguedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
