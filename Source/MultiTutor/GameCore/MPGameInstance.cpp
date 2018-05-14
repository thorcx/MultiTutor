// Fill out your copyright notice in the Description page of Project Settings.

#include "MPGameInstance.h"


UMPGameInstance::UMPGameInstance(const FObjectInitializer &Initializer)
{

}

void UMPGameInstance::Init()
{

}

void UMPGameInstance::Host()
{
	UWorld *world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UMPGameInstance::Join(FString &Address)
{

}
