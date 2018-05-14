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

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void AddActiveTrigger();

	void RemoveActiveTrigger();

	//移动速度
	UPROPERTY(EditAnywhere)
	float Speed;
	
	//蓝图内编辑的相对位置
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector TargetLocation;


private:
	//起始位置
	FVector GlobalStartLocation;

	//目标位置
	FVector GlobalTargetLocation;

	//朝向
	FVector MoveDirection;

	//总移动长度
	float TotalLength;

	UPROPERTY(EditAnywhere)
		int ActiveTriggers = 1;
};
