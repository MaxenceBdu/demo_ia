// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Public/Vehicle.h"
#include "Steering.generated.h"

UCLASS(Blueprintable)
class PROJET_STEERING_API ASteering : public AActor
{
	GENERATED_BODY()
	bool Arrived;
public:	
	UPROPERTY(EditAnywhere)
		AVehicle* Cube1;
	UPROPERTY(EditAnywhere)
		AVehicle* Cube2;
	UPROPERTY(EditAnywhere)
		bool Paused;
	
	// Sets default values for this actor's properties
	ASteering();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangePaused()
	{
		Paused = !Paused;
	}
private:
	
	FVector Seek(FVector Cube1Location, FVector Cube2Location)
	{
		FVector normalized = Cube2Location - Cube1Location;
		normalized.Normalize();
		return (normalized * Cube1->MaxSpeed) - Cube1->GetVelocity();
		/*
		UMovementComponent* MovementComponent = Cast<UMovementComponent>(Cube1->GetComponentByClass(UFloatingPawnMovement::StaticClass()));
		if(MovementComponent != nullptr)
		{
			
			MovementComponent->Velocity = 
		}*/
	}

};
