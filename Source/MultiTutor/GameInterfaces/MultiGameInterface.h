// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMultiGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTITUTOR_API IMultiGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//建主机
	virtual void HostMPGame() = 0;

	virtual void JoinMPGame() = 0;
};
