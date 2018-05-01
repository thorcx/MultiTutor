﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DemoCar.generated.h"

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
};

//服务器返回客户端的最新模拟后的状态
USTRUCT()
struct FDemoCarState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FDemoCarMove LastMove;
	
	UPROPERTY()
	FVector Velocity;
	
	UPROPERTY()
	FTransform Transform;
};


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

	////服务器移动
	//UFUNCTION(Server, Reliable, WithValidation)
	//void Server_MoveForward(float value);
	
	//UFUNCTION(Server, Reliable, WithValidation)
	//void Server_MoveRight(float value);

	//客户端发起RPC,服务器端执行，服务器获得Move信息
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FDemoCarMove Move);

private:

	void SimulateMove(const FDemoCarMove& Move);

	FDemoCarMove CreateMove(float DeltaTime);

	//从未响应列表中清除已经从服务器上获得的Move数据
	void ClearAcknowledgeMoves(FDemoCarMove LastMove);

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


	//油门
	//为了在ListenServer上尽可能接近的反应用户的输入，将油门与方向盘都同步
	//有了Move结构后不需要单独设置这两个变量同步了,整个结构体被填充后同步
	//UPROPERTY(replicated)
	float Throttle;

	//UPROPERTY(replicated)
	float SteeringThrow;
	
	//车的速度，矢量
	FVector Velocity;

	//UPROPERTY(ReplicatedUsing= OnRep_ReplicatedTransform)
	//FVector ReplicatedLocation;

	//UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)
	//FTransform ReplicatedTransform;

	//UFUNCTION()
	//void OnRep_ReplicatedTransform();

	UPROPERTY(replicated)
	FRotator ReplicatedRotation;

	UPROPERTY(replicated,ReplicatedUsing = OnRep_ServerState)
	FDemoCarState ServerState;

	//注意这里OnRep结构体的时候，结构体所有需要被Rep的属性必须是UPROPERTY,不加的话会同步不到值
	UFUNCTION()
	void OnRep_ServerState();

	//尚未相应的Move
	TArray<FDemoCarMove>	UnacknowledgedMoves;
};
