// Fill out your copyright notice in the Description page of Project Settings.


#include "Steering.h"

// Sets default values
ASteering::ASteering()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASteering::BeginPlay()
{
	Super::BeginPlay();
	const UMovementComponent* MovementComponent = Cast<UMovementComponent>(Cube1->GetComponentByClass(UFloatingPawnMovement::StaticClass()));
	if(MovementComponent != nullptr)
		Cube1MaxSpeed = MovementComponent->GetMaxSpeed();
}

// Called every frame
void ASteering::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Seek();

}

