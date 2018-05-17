// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"



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
	if (QuitGame)
	{
		QuitGame->OnClicked.AddDynamic(this, &UMainMenu::QuitMainGame);
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

void UMainMenu::QuitMainGame()
{
	UWorld *World = GetWorld();
	if (World == nullptr) return;
	APlayerController *Controller = World->GetFirstPlayerController();
	Controller->ConsoleCommand("quit");
}
