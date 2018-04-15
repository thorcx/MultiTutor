// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCar.h"
#include "Components/InputComponent.h"

// Sets default values
ADemoCar::ADemoCar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADemoCar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADemoCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetResistance();

	FVector Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaTime;

	

	ApplyRotation(DeltaTime);


	UpdateLocation(DeltaTime);

}

FVector ADemoCar::GetResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void ADemoCar::ApplyRotation(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * DeltaTime *steeringThrow;

	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

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
}

void ADemoCar::MoveRight(float value)
{
	steeringThrow = value;
}
