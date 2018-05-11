// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatformBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class MULTITUTOR_API AMovingPlatformBaseActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
	
public:

	AMovingPlatformBaseActor();

	virtual void Tick(float DeltaTime) override;
	
	
};
