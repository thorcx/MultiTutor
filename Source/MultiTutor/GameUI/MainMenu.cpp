// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"



void UMainMenu::SetMGInterface(IMultiGameInterface *Interface)
{
	MGInterface = Interface;
}

void UMainMenu::Setup()
{
	AddToViewport();
	UWorld *world = GetWorld();
	if (world == nullptr) return;
	APlayerController *controller = world->GetFirstPlayerController();
	if (controller)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		controller->SetInputMode(InputModeData);
		controller->bShowMouseCursor = true;
	}
}

void UMainMenu::TearDown()
{
	RemoveFromViewport();
	UWorld *world = GetWorld();
	if (world == nullptr) return;
	APlayerController *controller = world->GetFirstPlayerController();
	if (controller)
	{
		FInputModeGameOnly InputModeData;
		//InputModeData.SetWidgetToFocus(this->TakeWidget());
		//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		controller->SetInputMode(InputModeData);
		controller->bShowMouseCursor = false;
	}
}

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);
	}
	return true;
}

void UMainMenu::OnHostClicked()
{
	if (MGInterface)
	{
		MGInterface->HostMPGame();
	}
}
