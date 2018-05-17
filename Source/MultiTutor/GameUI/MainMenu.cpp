// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"



void UMainMenu::SetMGInterface(IMultiGameInterface *Interface)
{
	MGInterface = Interface;
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
