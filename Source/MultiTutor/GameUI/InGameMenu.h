// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBaseWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API UInGameMenu : public UMenuBaseWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	class UButton *Cancel;

	UPROPERTY(meta = (BindWidget))
	class UButton *Quit;
	
	UFUNCTION()
	void QuitGame();
	
	
	UFUNCTION()
	void ReturnToGame();
};
