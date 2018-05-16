// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;
private:
	//注意这里的绑定原语,名称要跟蓝图保持一致
	UPROPERTY(meta = (BindWidget))
	class UButton *HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton *JoinButton;

	UFUNCTION()
	void OnHostClicked();

};
