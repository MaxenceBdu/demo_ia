// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

#include "SAdvancedRotationInputBox.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Velocity = FVector(0.0,0.0,0.0);
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector SteeringForce = (Velocity * MaxForce)/Velocity.Length(); // truncate ( steering_direction , max_force );
	const FVector Acceleration = SteeringForce / Mass;
	Velocity = (Velocity+Acceleration) * MaxSpeed / Velocity.Length(); // truncate ( velocity + acceleration , max_speed );

	SetActorLocation(GetActorLocation()+Velocity);
	Velocity.Normalize();
	SetActorRotation(FRotationMatrix::MakeFromZ(Velocity).Rotator());

}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

