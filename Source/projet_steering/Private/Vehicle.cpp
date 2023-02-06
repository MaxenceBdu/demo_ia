// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Algo/Reverse.h"

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
	Velocity = FVector(0,0,0);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Circuit.Num() == 0 && Target == nullptr)
		Velocity = FVector::Zero();

	// If in Circuit/One way/Two ways mode
	if(Circuit.Num() > 0)
	{
		// Circuit Mode 0 = Circuit
		// Circuit Mode 1 = OneWay
		// Circuit Mode 2 = TwoWays
		
		// And if target is reached, change target
		if(GetActorLocation().Equals(Target->GetActorLocation(), 30.0))
		{
			if(TargetId == Circuit.Num()-1)
			{
				if(CircuitMode == 1) // one way, stop at the end of the path
				{
					Target = nullptr;
				}else
				{
					if(CircuitMode == 2) // two ways, follow the path backwards
						Algo::Reverse(Circuit);
					TargetId = 0; // reset circuit
				}
			}else
			{
				if(TargetId == Circuit.Num()-2 && CircuitMode != 0)
					// when aiming for the last target of the path, switch to Arrival()
					MovementFunction = &AVehicle::Arrival;
				else if(TargetId == 0 && CircuitMode == 2) // MovementFunction previously was Arrival() so we're back to Seek()
					MovementFunction = &AVehicle::Seek;
				++TargetId;
			}
			Target = Circuit[TargetId];
		}
	}
	
	if(MovementFunction != nullptr && Target != nullptr)
	{
		// Work out the velocity
		const FVector SteeringForce = (this->* MovementFunction)().GetClampedToMaxSize(MaxForce); // maximum magnitude clamped to MaxForce
		const FVector Acceleration = SteeringForce / Mass;

		Velocity = (Velocity+Acceleration).GetClampedToMaxSize(MaxSpeed); // maximum magnitude clamped to MaxSpeed
	}

	// Update position and rotation of the vehicle
	SetActorLocation(GetActorLocation()+Velocity, true);
	FVector Normalized = FVector(Velocity);
	Normalized.Normalize();
	FRotator Rot = FRotationMatrix::MakeFromX(Normalized).Rotator();
	SetActorRotation(FRotator(-90, Rot.Yaw, Rot.Roll));
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
	const double Distance = FVector::DistXY(Target->GetActorLocation(), GetActorLocation());
	FVector Normalized = Target->GetActorLocation()+Target->GetVelocity()*(Distance/MaxSpeed) - GetActorLocation();
	Normalized.Normalize();
	return Normalized * MaxSpeed - Velocity;
}

FVector AVehicle::Evade()
{
	return Pursuit()*-1;
}

FVector AVehicle::Arrival()
{
	const FVector TargetOffset = Target->GetActorLocation() - GetActorLocation();
	const float Distance = TargetOffset.Size();
	const float RampedSpeed = MaxSpeed * ( Distance / SlowingDistance );
	
	float ClippedSpeed;
	if(RampedSpeed < MaxSpeed)
		ClippedSpeed = RampedSpeed;
	else
		ClippedSpeed = MaxSpeed;
	
	const FVector DesiredVelocity = ClippedSpeed / Distance * TargetOffset ;
	return DesiredVelocity - Velocity ;
}
