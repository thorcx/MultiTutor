// Fill out your copyright notice in the Description page of Project Settings.

#include "MPGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameUI/MainMenu.h"
#include "GameUI/MenuBaseWidget.h"
UMPGameInstance::UMPGameInstance(const FObjectInitializer &Initializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (MenuBPClass.Class)
	{
		MenuClass = MenuBPClass.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UI/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class)
	{
		InGameMenuClass = InGameMenuBPClass.Class;
	}
}

void UMPGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UMPGameInstance::HostMPGame()
{
	Host();
}

void UMPGameInstance::JoinMPGame(FString &IPAddress)
{
	Join(IPAddress);
}

void UMPGameInstance::LoadMainMenu()
{
	APlayerController *controller = GetFirstLocalPlayerController();
	if (controller)
	{
		controller->ClientTravel("/Game/Maps/MainMenuLevel", ETravelType::TRAVEL_Absolute);
	}
}

void UMPGameInstance::LoadMenu()
{
	if (MenuClass != nullptr)
	{
		MenuInstance = CreateWidget<UMenuBaseWidget>(this, MenuClass);
		MenuInstance->Setup();
		MenuInstance->SetMGInterface(this);
	}
}

void UMPGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass)
	{
		MenuInstance = CreateWidget<UMenuBaseWidget>(this, InGameMenuClass);
		MenuInstance->Setup();
		MenuInstance->SetMGInterface(this);
	}
}

void UMPGameInstance::Host()
{
	if (MenuInstance != nullptr)
	{
		MenuInstance->TearDown();
	}
	UWorld *world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UMPGameInstance::Join(FString &Address)
{
	if (MenuInstance != nullptr)
	{
		MenuInstance->TearDown();
	}
	APlayerController *controller = GetFirstLocalPlayerController();
	if (controller)
	{
		controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}
