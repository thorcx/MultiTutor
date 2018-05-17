// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInterfaces/MultiGameInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMGInterface(IMultiGameInterface *Interface);
	
	void Setup();

	void TearDown();
protected:

	virtual bool Initialize() override;
private:
	//注意这里的绑定原语,名称要跟蓝图保持一致
	UPROPERTY(meta = (BindWidget))
	class UButton *HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton *JoinButton;

	//二级菜单的Join
	UPROPERTY(meta = (BindWidget))
	class UButton *Join;
	
	//二级菜单的Cancel
	UPROPERTY(meta = (BindWidget))
	class UButton *Cancel;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher *MenuSwitcher; 

	UPROPERTY(meta = (BindWidget))
	class UWidget *JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget *MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox *IP_TB;

	UFUNCTION()
	void OnHostClicked();

	//主菜单内Join
	UFUNCTION()
	void OnJoinInMainClicked();

	UFUNCTION()
	void JoinGame();

	UFUNCTION()
	void BackToMain();

	IMultiGameInterface* MGInterface;
};
