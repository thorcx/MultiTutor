// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DemoCar.generated.h"

UCLASS()
class MULTITUTOR_API ADemoCar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADemoCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float value);
	
	void MoveRight(float value);

private:

	FVector GetResistance();

	void UpdateLocation(float DeltaTime);

	void ApplyRotation(float DeltaTime);

private:

	//车的重量, 单位kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	//牵引力，单位N(牛顿),当油门全力后的最大值
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	//空气阻力系数,假设空气阻力最大为MaxDrivingForce，这里是10000，而速度要达到25米/秒
	//用公式 空气阻力 = -(速度)^2 * 系数，得出为16
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//油门
	float Throttle;

	float steeringThrow;
	
	FVector Velocity;
};
