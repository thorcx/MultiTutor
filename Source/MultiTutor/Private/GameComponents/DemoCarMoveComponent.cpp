// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCarMoveComponent.h"


// Sets default values for this component's properties
UDemoCarMoveComponent::UDemoCarMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDemoCarMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDemoCarMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	/*FDemoCarMove Move = CreateMove(DeltaTime);
	SimulateMove(Move);*/
}

void UDemoCarMoveComponent::SimulateMove(const FDemoCarMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();

	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity += Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.steeringThrow);

	UpdateLocation(Move.DeltaTime);

}

FDemoCarMove UDemoCarMoveComponent::CreateMove(float DeltaTime)
{
	FDemoCarMove Move;
	Move.DeltaTime = DeltaTime;
	Move.steeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;
	//todo time
	return Move;
}

void UDemoCarMoveComponent::SetVelocity(FVector Velocity)
{
	this->Velocity = Velocity;
}

FVector UDemoCarMoveComponent::GetVelocity()
{
	return this->Velocity;
}

void UDemoCarMoveComponent::SetThrottle(float Throttle)
{
	this->Throttle = Throttle;
}

float UDemoCarMoveComponent::GetThrottle()
{
	return this->Throttle;
}

void UDemoCarMoveComponent::SetSteeringThrow(float SteeringThrow)
{
	this->SteeringThrow = SteeringThrow;
}

float UDemoCarMoveComponent::GetSteeringThrow()
{
	return this->SteeringThrow;
}

FVector UDemoCarMoveComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UDemoCarMoveComponent::GetRollingResistance()
{
	float g = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * g;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UDemoCarMoveComponent::ApplyRotation(float DeltaTime, float SteeringThrow)
{
	//float RotationAngle = MaxDegreesPerSecond * DeltaTime *SteeringThrow;

	//这里的公式是∆x = ∆(θ) * r;注意这里求出的theta 已经是弧度了
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;

	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;

	//UE_LOG(LogTemp, Warning, TEXT("Rot angle is %f"), RotationAngle);

	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UDemoCarMoveComponent::UpdateLocation(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;

	FHitResult HitResult;

	GetOwner()->AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}