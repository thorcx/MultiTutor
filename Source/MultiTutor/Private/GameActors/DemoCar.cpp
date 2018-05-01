// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCar.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
// Sets default values
ADemoCar::ADemoCar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ADemoCar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ADemoCar, ReplicatedTransform);
	//DOREPLIFETIME(ADemoCar, Velocity);
	DOREPLIFETIME(ADemoCar, ServerState);
	//DOREPLIFETIME(ADemoCar, Throttle);
	//DOREPLIFETIME(ADemoCar, SteeringThrow);
	//DOREPLIFETIME(ADemoCar, ReplicatedLocation);
	//DOREPLIFETIME(ADemoCar, ReplicatedRotation);
}


// Called when the game starts or when spawned
void ADemoCar::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

//Debug用
FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	default:
		return "Error";
	}
}

// Called every frame
void ADemoCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//如果是Autonomous,创建这个Move，然后Send到Server
	//注意这里IsLocallyControlled()并不是确保当前代码是客户端的意思，而是确保这个Actor是被本地的Controller Process,也就是当前运行进程内的Controller
	//而不是一个network Controller
	//if (IsLocallyControlled())
	//{
	//	
	//	FDemoCarMove Move = CreateMove(DeltaTime);
	//	//这里的判断确保代码只在非服务器上执行
	//	if (!HasAuthority())
	//	{
	//		UnacknowledgedMoves.Add(Move);
	//		SimulateMove(Move);

	//		UE_LOG(LogTemp, Warning, TEXT("Queue length:%d"), UnacknowledgedMoves.Num());
	//	
	//	}
	//	
	//	
	//	Server_SendMove(Move);
	//}
	if (Role == ROLE_AutonomousProxy)
	{
		FDemoCarMove Move = CreateMove(DeltaTime);
		UnacknowledgedMoves.Add(Move);
		SimulateMove(Move);
		Server_SendMove(Move);
	}
	//代表当前代码在Server端，并且是正在屏幕上控制的角色
	if (Role == ROLE_Authority && GetRemoteRole() == ROLE_SimulatedProxy)
	{
		FDemoCarMove Move = CreateMove(DeltaTime);
		Server_SendMove(Move);
	}
	
	if (Role == ROLE_SimulatedProxy)
	{
		SimulateMove(ServerState.LastMove);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Gravity is %f"), GetWorld()->GetGravityZ());

	//float g = -GetWorld()->GetGravityZ() / 100;

	

	//放入ServerSimulateMove之后，不在Tick内做同步
	//if (HasAuthority())
	//{
		//ReplicatedLocation = GetActorLocation();
		//ReplicatedRotation = GetActorRotation();
	//	ServerState.Transform = GetActorTransform();
	//	ServerState.Velocity = Velocity;
	//}
	//else {
	//	SetActorTransform(ReplicatedTransform);

	//	//SetActorLocation(ReplicatedLocation);
	//	//SetActorRotation(ReplicatedRotation);
	//}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::Red, DeltaTime);

}



void ADemoCar::SimulateMove(const FDemoCarMove& Move)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();

	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity += Acceleration * Move.DeltaTime;
	
	ApplyRotation(Move.DeltaTime, Move.steeringThrow);

	UpdateLocation(Move.DeltaTime);

}

FDemoCarMove ADemoCar::CreateMove(float DeltaTime)
{
	FDemoCarMove Move;
	Move.DeltaTime = DeltaTime;
	Move.steeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;
	//todo time
	return Move;
}

void ADemoCar::ClearAcknowledgeMoves(FDemoCarMove LastMove)
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

FVector ADemoCar::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector ADemoCar::GetRollingResistance()
{
	float g = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * g;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void ADemoCar::ApplyRotation(float DeltaTime, float SteeringThrow)
{
	//float RotationAngle = MaxDegreesPerSecond * DeltaTime *SteeringThrow;

	//这里的公式是∆x = ∆(θ) * r;注意这里求出的theta 已经是弧度了
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;

	//UE_LOG(LogTemp, Warning, TEXT("Rot angle is %f"), RotationAngle);

	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

//在Tick内服务器更新位置，每间隔1段时间发送一个同步位置回客户端，客户端在间隔期内依赖自己的Tick更新位置，当
//RepNofity回来后，刷新服务器位置来同步一次
//void ADemoCar::OnRep_ReplicatedTransform()
//{
//	SetActorTransform(ReplicatedTransform);
//	UE_LOG(LogTemp, Warning, TEXT("Replicated Location"));
//}

void ADemoCar::OnRep_ServerState()
{
	
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;
	//UE_LOG(LogTemp, Warning, TEXT("From Server time is %f"), ServerState.LastMove.Time);
	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FDemoCarMove& Move : UnacknowledgedMoves)
	{
		SimulateMove(Move);
	}

}

void ADemoCar::UpdateLocation(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;

	FHitResult HitResult;

	AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
void ADemoCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADemoCar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADemoCar::MoveRight);
}

void ADemoCar::MoveForward(float value)
{
	Throttle = value;
	//Server_MoveForward(value);
}

void ADemoCar::MoveRight(float value)
{
	SteeringThrow = value;
	//Server_MoveRight(value);
}

//这里的代码总是在服务器上执行
void ADemoCar::Server_SendMove_Implementation(FDemoCarMove Move)
{
	


	//Server接到信息后不直接同步，而是SimulateMove
	/*Throttle = Move.Throttle;
	SteeringThrow = Move.steeringThrow;*/
	SimulateMove(Move);
	//完成模拟后，将服务器状态发送回Client
	//注意这里不再需要单独Replicate Throttle,SteerThrow变量
	ServerState.LastMove = Move;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;

	//UE_LOG(LogTemp, Warning, TEXT("ServerState Velocity is %f,%f,%f"), Velocity.X, Velocity.Y,Velocity.Z);
}

bool ADemoCar::Server_SendMove_Validate(FDemoCarMove Move)
{
	return true;
}

//void ADemoCar::Server_MoveForward_Implementation(float value)
//{
//	Throttle = value;
//}
//bool ADemoCar::Server_MoveForward_Validate(float value)
//{
//	return FMath::Abs(value) <= 1.0f;
//}
//
//void ADemoCar::Server_MoveRight_Implementation(float value)
//{
//	SteeringThrow = value;
//}
//
//bool ADemoCar::Server_MoveRight_Validate(float value)
//{
//	return FMath::Abs(value) <= 1.0f;
//}

