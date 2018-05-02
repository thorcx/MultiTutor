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
		//MovementComponent->SimulateMove(ServerState.LastMove);
		ClientTick(DeltaTime);
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

void UDemoCarReplicateComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER)
	{
		return;
	}
	FVector TargetLocation = ServerState.Transform.GetLocation();
	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	FVector StartLocation = ClientStartLocation;
	FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);

	GetOwner()->SetActorLocation(NewLocation);
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
	//当服务器状态被Rep后，这里区分两种情况，如果是Autonomous_proxy，我们进行unacknowledgeMove的模拟
	//如果是SimulatedProxy,我们记录下上次收到Update的间隔时间,然后重新开始计时，然后在客户端模拟前两次收到
	//Server消息的时间间隔内物体的位移，使用Lerp进行线性插值计算,所以这里服务器的信息更新到客户端总是慢一个
	//ServerUpdate的间隔，虽然同步性稍差些，但是在客户端看物体的位移会很平滑
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRep_ServerState();
		break;
	default:
		break;
	}

}

void UDemoCarReplicateComponent::SimulatedProxy_OnRep_ServerState()
{
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	ClientStartLocation = GetOwner()->GetActorLocation();
}

void UDemoCarReplicateComponent::AutonomousProxy_OnRep_ServerState()
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
