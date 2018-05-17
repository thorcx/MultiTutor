// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInterfaces/MultiGameInterface.h"
#include "MPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API UMPGameInstance : public UGameInstance, public IMultiGameInterface
{
	GENERATED_BODY()

public:
	UMPGameInstance(const FObjectInitializer &Initializer);

	virtual void Init();
	
	//~BeginInterface
	virtual void HostMPGame() override;

	virtual void JoinMPGame(FString &IPAddress) override;

	virtual void LoadMainMenu() override;
	//~End Interface

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(FString &Address);

private:
	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	//当前菜单指针
	class UMenuBaseWidget* MenuInstance;
};
