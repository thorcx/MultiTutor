// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Public/GameComponents/DemoCarMoveComponent.h"
#include "Public/GameComponents/DemoCarReplicateComponent.h"
#include "DemoCar.generated.h"






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

	

private:


	

	


	
	

	//UPROPERTY(ReplicatedUsing= OnRep_ReplicatedTransform)
	//FVector ReplicatedLocation;

	//UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)
	//FTransform ReplicatedTransform;

	//UFUNCTION()
	//void OnRep_ReplicatedTransform();

	

	

	UPROPERTY(VisibleAnywhere)
	UDemoCarMoveComponent*	MovementComponent;

	UPROPERTY(VisibleAnywhere)
	UDemoCarReplicateComponent* MovementReplicator;
};
