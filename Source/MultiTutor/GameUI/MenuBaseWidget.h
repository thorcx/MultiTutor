// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInterfaces/MultiGameInterface.h"
#include "MenuBaseWidget.generated.h"

/**
 * 菜单公用的显示，销毁功能
 */
UCLASS()
class MULTITUTOR_API UMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMGInterface(IMultiGameInterface *Interface);

	void Setup();

	void TearDown();
	
protected:


	IMultiGameInterface* MGInterface;
	
};
