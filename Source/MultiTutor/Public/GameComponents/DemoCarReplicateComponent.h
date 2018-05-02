// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/GameComponents/DemoCarMoveComponent.h"
#include "DemoCarReplicateComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTITUTOR_API UDemoCarReplicateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDemoCarReplicateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//从未响应列表中清除已经从服务器上获得的Move数据
	void ClearAcknowledgeMoves(FDemoCarMove LastMove);
	
	void UpdateServerState(const FDemoCarMove& Move);

	//客户端发起RPC,服务器端执行，服务器获得Move信息
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FDemoCarMove Move);

	void ClientTick(float DeltaTime);
	
	UPROPERTY(replicated)
	FRotator ReplicatedRotation;

	UPROPERTY(replicated, ReplicatedUsing = OnRep_ServerState)
	FDemoCarState ServerState;

	//注意这里OnRep结构体的时候，结构体所有需要被Rep的属性必须是UPROPERTY,不加的话会同步不到值
	UFUNCTION()
	void OnRep_ServerState();

	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();

	//尚未得到服务器响应的Move的集合
	TArray<FDemoCarMove>	UnacknowledgedMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FVector ClientStartLocation;


	UPROPERTY()
	UDemoCarMoveComponent*	MovementComponent;

};
