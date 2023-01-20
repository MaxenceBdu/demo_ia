// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

#include "SAdvancedRotationInputBox.h"

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
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Arrived)
	{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Arrived ok"));
		if(MovementFunction != nullptr)
		{
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Movement ok"));
			if(Target != nullptr)
			{
				if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Target ok"));
				// Update position and rotation of the vehicle
				const FVector SteeringForce = (this->* MovementFunction)() * MaxForce / Velocity.Length(); // truncate ( steering_direction , max_force );
				const FVector Acceleration = SteeringForce / Mass;

				Velocity = (Velocity+Acceleration) * MaxSpeed / Velocity.Length(); // truncate ( velocity + acceleration , max_speed );
				SetActorLocation(GetActorLocation()+Velocity);
				FVector Normalized = FVector(Velocity);
				Normalized.Normalize();
				SetActorRotation(FRotationMatrix::MakeFromX(Normalized).Rotator());
			}
		}
	}
		
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Steering functions

FVector AVehicle::Seek()
{
	if(Target->GetActorLocation() == GetActorLocation()){
		Arrived = true;
		return Zero;
	}
	
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

