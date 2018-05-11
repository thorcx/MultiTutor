// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformBaseActor.h"



AMovingPlatformBaseActor::AMovingPlatformBaseActor()
	:Speed(20.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformBaseActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatformBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
	{
		FVector location = GetActorLocation();
		location += FVector(Speed * DeltaTime, 0.0f, 0.0f);
		SetActorLocation(location);
	}
}
