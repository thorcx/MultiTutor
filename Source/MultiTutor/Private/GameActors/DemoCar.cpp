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
	DOREPLIFETIME(ADemoCar, ReplicatedLocation);
}


// Called when the game starts or when spawned
void ADemoCar::BeginPlay()
{
	Super::BeginPlay();
	
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

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetAirResistance();

	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaTime;

	//UE_LOG(LogTemp, Warning, TEXT("Gravity is %f"), GetWorld()->GetGravityZ());

	//float g = -GetWorld()->GetGravityZ() / 100;

	ApplyRotation(DeltaTime);


	UpdateLocation(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedLocation = GetActorLocation();
		ReplicatedRotation = GetActorRotation();
	}
	else {
		SetActorLocation(ReplicatedLocation);
		SetActorRotation(ReplicatedRotation);
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::Red, DeltaTime);

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

void ADemoCar::ApplyRotation(float DeltaTime)
{
	//float RotationAngle = MaxDegreesPerSecond * DeltaTime *steeringThrow;

	//这里的公式是∆x = ∆(θ) * r;注意这里求出的theta 已经是弧度了
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinTurningRadius * steeringThrow;

	UE_LOG(LogTemp, Warning, TEXT("Rot angle is %f"), RotationAngle);

	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
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
	Server_MoveForward(value);
}

void ADemoCar::MoveRight(float value)
{
	steeringThrow = value;
	Server_MoveRight(value);
}

void ADemoCar::Server_MoveForward_Implementation(float value)
{
	Throttle = value;
}
bool ADemoCar::Server_MoveForward_Validate(float value)
{
	return FMath::Abs(value) <= 1.0f;
}

void ADemoCar::Server_MoveRight_Implementation(float value)
{
	steeringThrow = value;
}

bool ADemoCar::Server_MoveRight_Validate(float value)
{
	return FMath::Abs(value) <= 1.0f;
}

