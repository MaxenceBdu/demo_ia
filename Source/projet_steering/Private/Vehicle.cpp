// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	Arrived = false;
	Velocity = FVector(0,0,0);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Arrived && MovementFunction != nullptr && Target != nullptr)
	{
		// Update position and rotation of the vehicle
		const FVector SteeringForce = (this->* MovementFunction)().GetClampedToMaxSize(MaxForce); // maximum magnitude clamped to MaxForce
		const FVector Acceleration = SteeringForce / Mass;

		Velocity = (Velocity+Acceleration).GetClampedToMaxSize(MaxSpeed); // maximum magnitude clamped to MaxSpeed
	}
	
	SetActorLocation(GetActorLocation()+Velocity);
	FVector Normalized = FVector(Velocity);
	Normalized.Normalize();
	SetActorRotation(FRotationMatrix::MakeFromX(Normalized).Rotator());
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Steering functions

FVector AVehicle::Seek()
{
	FVector Normalized = Target->GetActorLocation() - GetActorLocation();
	Normalized.Normalize();
	return Normalized * MaxSpeed - Velocity;	
}

FVector AVehicle::Flee()
{
	return Seek()*-1;
}

FVector AVehicle::Pursuit()
{
	FVector Normalized = Target->GetActorLocation()+Target->GetVelocity()*Interval - GetActorLocation();
	Normalized.Normalize();
	return Normalized * MaxSpeed - Velocity;
}

FVector AVehicle::Evade()
{
	return Velocity;
}

FVector AVehicle::Arrival()
{
	return Velocity;
}
