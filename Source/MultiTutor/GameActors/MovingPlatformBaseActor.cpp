// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformBaseActor.h"



AMovingPlatformBaseActor::AMovingPlatformBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		FVector location = GetActorLocation();
		location += FVector(3.0f * DeltaTime, 0.0f, 0.0f);
		SetActorLocation(location);
	}
}
