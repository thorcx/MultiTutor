// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MultiTutorGameMode.h"
#include "MultiTutorPawn.h"
#include "MultiTutorHud.h"

AMultiTutorGameMode::AMultiTutorGameMode()
{
	DefaultPawnClass = AMultiTutorPawn::StaticClass();
	HUDClass = AMultiTutorHud::StaticClass();
}
