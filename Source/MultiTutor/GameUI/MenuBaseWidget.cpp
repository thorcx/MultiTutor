// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuBaseWidget.h"




void UMenuBaseWidget::SetMGInterface(IMultiGameInterface *Interface)
{
	MGInterface = Interface;
}

void UMenuBaseWidget::Setup()
{
	AddToViewport();
	UWorld *World = GetWorld();
	if (World == nullptr) return;
	APlayerController *Controller = World->GetFirstPlayerController();
	if (Controller)
	{
		FInputModeUIOnly InputData;
		InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputData.SetWidgetToFocus(this->TakeWidget());
		Controller->SetInputMode(InputData);
		Controller->bShowMouseCursor = true;
	}

}

void UMenuBaseWidget::TearDown()
{
	RemoveFromViewport();
	UWorld *World = GetWorld();
	if (World == nullptr) return;
	APlayerController *Controller = World->GetFirstPlayerController();
	if (Controller)
	{
		FInputModeGameOnly InputData;
		Controller->SetInputMode(InputData);
		Controller->bShowMouseCursor = false;
	}
}
