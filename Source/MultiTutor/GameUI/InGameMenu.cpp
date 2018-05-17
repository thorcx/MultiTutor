// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"



bool UInGameMenu::Initialize()
{
	Super::Initialize();

	if (Cancel)
	{
		Cancel->OnClicked.AddDynamic(this, &UInGameMenu::ReturnToGame);
	}

	if (Quit)
	{
		Quit->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);
	}
	return true;
}

void UInGameMenu::QuitGame()
{
	if (MGInterface)
	{
		TearDown();
		MGInterface->LoadMainMenu();
	}
}

void UInGameMenu::ReturnToGame()
{
	TearDown();
}
