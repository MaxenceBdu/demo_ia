// Fill out your copyright notice in the Description page of Project Settings.


#include "Steering.h"

// Sets default values
ASteering::ASteering()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Paused = true;
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
	
	if(!Paused)
		Cube1->Velocity = Seek(Cube1->GetActorLocation(), Cube2->GetActorLocation());
}

