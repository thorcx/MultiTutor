// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinInMainClicked);
	}
	if (Join)
	{
		Join->OnClicked.AddDynamic(this, &UMainMenu::JoinGame);
	}
	if (Cancel)
	{
		Cancel->OnClicked.AddDynamic(this, &UMainMenu::BackToMain);
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

void UMainMenu::OnJoinInMainClicked()
{
	if(MenuSwitcher)
		MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::JoinGame()
{
	FString Ip = IP_TB->GetText().ToString();
	if (MGInterface)
	{
		MGInterface->JoinMPGame(Ip);
	}
}

void UMainMenu::BackToMain()
{
	if (MenuSwitcher)
		MenuSwitcher->SetActiveWidget(MainMenu);
}
