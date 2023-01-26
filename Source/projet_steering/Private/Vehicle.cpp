// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "GenericPlatform/GenericPlatformMath.h"

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

	if(Circuit.Num() == 0 && Target == nullptr)
		Velocity = FVector::Zero();

	if(Circuit.Num() > 0)
	{
		if(GetActorLocation() == Target->GetActorLocation())
		{
			Target = Circuit[++TargetId];
		}
		
	}
	
	if(!Arrived && MovementFunction != nullptr && Target != nullptr)
	{
		// Update position and rotation of the vehicle
		const FVector SteeringForce = (this->* MovementFunction)().GetClampedToMaxSize(MaxForce); // maximum magnitude clamped to MaxForce
		const FVector Acceleration = SteeringForce / Mass;

		Velocity = (Velocity+Acceleration).GetClampedToMaxSize(MaxSpeed); // maximum magnitude clamped to MaxSpeed
	}
	
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
	double Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	/*FVector NormalizedVelocity = Velocity, NormalizedTargetVelocity = Target->GetVelocity();
	NormalizedVelocity.Normalize();
	NormalizedTargetVelocity.Normalize();
	
	double Angle = FVector::DotProduct(NormalizedVelocity, NormalizedTargetVelocity) / (Velocity.Size()*Target->GetVelocity().Size());
	
	FVector Normalized = Target->GetActorLocation()+Target->GetVelocity()*(Distance*Angle) - GetActorLocation();*/
	FVector Normalized = Target->GetActorLocation()+Target->GetVelocity()*Distance - GetActorLocation();
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
