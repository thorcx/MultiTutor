// Fill out your copyright notice in the Description page of Project Settings.

#include "MPGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
UMPGameInstance::UMPGameInstance(const FObjectInitializer &Initializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (MenuBPClass.Class)
	{
		MenuClass = MenuBPClass.Class;
	}
}

void UMPGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UMPGameInstance::LoadMenu()
{
	if (MenuClass != nullptr)
	{
		UUserWidget *mainMenu = CreateWidget<UUserWidget>(this, MenuClass);
		mainMenu->AddToViewport();
	}
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
	APlayerController *controller = GetFirstLocalPlayerController();
	if (controller)
	{
		controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}
