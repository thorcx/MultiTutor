// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API UMPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMPGameInstance(const FObjectInitializer &Initializer);

	virtual void Init();
	
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(FString &Address);

private:
	TSubclassOf<class UUserWidget> MenuClass;
};
