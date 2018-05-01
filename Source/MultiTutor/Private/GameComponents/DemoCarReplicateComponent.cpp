// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCarReplicateComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UDemoCarReplicateComponent::UDemoCarReplicateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UDemoCarReplicateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDemoCarReplicateComponent, ServerState);
	
}

// Called when the game starts
void UDemoCarReplicateComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UDemoCarMoveComponent>();
	
}


// Called every frame
void UDemoCarReplicateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(MovementComponent->GetLastMove());
		Server_SendMove(MovementComponent->GetLastMove());
	}
	//代表当前代码在Server端，并且是正在屏幕上控制的角色
	if ( GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(MovementComponent->GetLastMove());
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState.LastMove);
	}

}

void UDemoCarReplicateComponent::ClearAcknowledgeMoves(FDemoCarMove LastMove)
{
	TArray<FDemoCarMove> NewMoves;
	//这里做的是当本地模拟的时间戳>服务器传回来的时间戳，我们把服务器当前时间点
	//之后的所有Move重新保存到unacknowledgedMove数组内
	for (const FDemoCarMove& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;
}

void UDemoCarReplicateComponent::UpdateServerState(const FDemoCarMove& Move)
{
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

//这里的代码总是在服务器上执行
void UDemoCarReplicateComponent::Server_SendMove_Implementation(FDemoCarMove Move)
{



	//Server接到信息后不直接同步，而是SimulateMove
	/*Throttle = Move.Throttle;
	SteeringThrow = Move.steeringThrow;*/
	MovementComponent->SimulateMove(Move);
	//完成模拟后，将服务器状态发送回Client
	//注意这里不再需要单独Replicate Throttle,SteerThrow变量
	UpdateServerState(Move);

	
}

bool UDemoCarReplicateComponent::Server_SendMove_Validate(FDemoCarMove Move)
{
	return true;
}


void UDemoCarReplicateComponent::OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);
	//UE_LOG(LogTemp, Warning, TEXT("From Server time is %f"), ServerState.LastMove.Time);
	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FDemoCarMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}

}
