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
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, Velocity.ToString());
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Arrived && MovementFunction != nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Red, Velocity.ToString());
		// Update position and rotation of the vehicle
		const FVector SteeringForce = (this->* MovementFunction)().GetClampedToMaxSize(MaxForce); // truncate ( steering_direction , max_force );
		const FVector Acceleration = SteeringForce / Mass;

		Velocity = (Velocity+Acceleration).GetClampedToMaxSize(MaxSpeed); // truncate ( velocity + acceleration , max_speed );
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
	/*if(Target == GetActorLocation()){
		Arrived = true;
		return FVector::Zero();
	}*/
	
	FVector Normalized = Target - GetActorLocation();
	Normalized.Normalize();
	FVector res = Normalized * MaxSpeed - Velocity;
	
	return res;
	
}

FVector AVehicle::Flee()
{
	return Seek()*-1;
}

FVector AVehicle::Pursuit()
{
	return Velocity;
}

FVector AVehicle::Evade()
{
	return Velocity;
}

FVector AVehicle::Arrival()
{
	return Velocity;
}
