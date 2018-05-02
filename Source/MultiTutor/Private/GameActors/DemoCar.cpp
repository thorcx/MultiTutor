// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCar.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADemoCar::ADemoCar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//去掉UE的自动Rep位置，让RepComponent来做位移Rep的差值计算,这样的好处是我们可以以很低的NetUpdateFrequency来获得相对平滑的服务器到客户端同步
	//如果不是这样做，除非把NetUpdateFrequency设置的很高，比如1秒100次，否则在客户端上看到的物体位移会出现抖动拉扯.
	bReplicateMovement = false;
	MovementComponent = CreateDefaultSubobject<UDemoCarMoveComponent>(TEXT("MovementCompnent"));
	MovementReplicator = CreateDefaultSubobject<UDemoCarReplicateComponent>(TEXT("MovementReplicator"));
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









//在Tick内服务器更新位置，每间隔1段时间发送一个同步位置回客户端，客户端在间隔期内依赖自己的Tick更新位置，当
//RepNofity回来后，刷新服务器位置来同步一次
//void ADemoCar::OnRep_ReplicatedTransform()
//{
//	SetActorTransform(ReplicatedTransform);
//	UE_LOG(LogTemp, Warning, TEXT("Replicated Location"));
//}




// Called to bind functionality to input
void ADemoCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADemoCar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADemoCar::MoveRight);
}

void ADemoCar::MoveForward(float value)
{
	MovementComponent->SetThrottle( value);
	//Server_MoveForward(value);
}

void ADemoCar::MoveRight(float value)
{
	MovementComponent->SetSteeringThrow(value);
	//Server_MoveRight(value);
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

