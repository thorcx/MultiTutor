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

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	TotalLength = (GlobalTargetLocation - GlobalStartLocation).Size();
}

void AMovingPlatformBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector currentLocation = GetActorLocation();
			float currentLength = (currentLocation - GlobalStartLocation).Size();

			if (currentLength >= TotalLength)
			{
				FVector temp = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = temp;
			}

			MoveDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			currentLocation += MoveDirection *Speed *DeltaTime;
			SetActorLocation(currentLocation);
		}
	}
}

void AMovingPlatformBaseActor::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatformBaseActor::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}
