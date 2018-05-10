// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DemoCarMoveComponent.generated.h"
//客户端预测要发送到服务器的Move
USTRUCT()
struct FDemoCarMove
{
	GENERATED_USTRUCT_BODY()
    UPROPERTY()
	float Throttle;
	UPROPERTY()
	float steeringThrow;
	UPROPERTY()
	float DeltaTime;

	//时间戳
	UPROPERTY()
	float Time;

	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(steeringThrow) <= 1;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTITUTOR_API UDemoCarMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDemoCarMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SimulateMove(const FDemoCarMove& Move);

	

	void SetVelocity(FVector Velocity);
	FVector GetVelocity();

	void SetThrottle(float Throttle);
	float GetThrottle();

	void SetSteeringThrow(float SteeringThrow);
	float GetSteeringThrow();

	FDemoCarMove GetLastMove() { return LastMove; };

private:
	FDemoCarMove CreateMove(float DeltaTime);
	//空气阻力
	FVector GetAirResistance();
	//滚转阻力
	FVector GetRollingResistance();

	void UpdateLocation(float DeltaTime);

	void ApplyRotation(float DeltaTime, float SteeringThrow);

private:

	//车的重量, 单位kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	//牵引力，单位N(牛顿),当油门全力后的最大值
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	//没用，最大每秒转弯角度
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	//汽车当方向盘打满后的最小转弯半径(单位米)，一般车为10
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;


	//空气阻力系数,假设空气阻力最大为MaxDrivingForce，这里是10000，而速度要达到25米/秒
	//用公式 空气阻力 = -(速度)^2 * 系数，得出为16
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//滚转阻力系数
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	//车的速度，矢量
	FVector Velocity;


	
	float Throttle;

	float SteeringThrow;

	FDemoCarMove	LastMove;
};
