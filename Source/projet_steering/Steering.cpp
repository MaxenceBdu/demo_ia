// Fill out your copyright notice in the Description page of Project Settings.


#include "Steering.h"

// Sets default values
ASteering::ASteering()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Paused = true;
	Mode = "";
}

// Called when the game starts or when spawned
void ASteering::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASteering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Paused && !Mode.Equals(""))
	{
		const FVector SteeringForce = (this->* MovementFunction)() * MaxForce / Velocity.Length(); // truncate ( steering_direction , max_force );
		const FVector Acceleration = SteeringForce / Mass;

		Velocity = (Velocity+Acceleration) * MaxSpeed / Velocity.Length(); // truncate ( velocity + acceleration , max_speed );
		Vehicle->SetActorLocation(Vehicle->GetActorLocation()+Velocity);
		FVector Normalized = FVector(Velocity);
		Normalized.Normalize();
		Vehicle->SetActorRotation(FRotationMatrix::MakeFromX(Normalized).Rotator());
	}
		
}

