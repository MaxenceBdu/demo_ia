// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Steering.generated.h"

UCLASS(Blueprintable)
class PROJET_STEERING_API ASteering : public AActor
{
	GENERATED_BODY()

	float Cube1MaxSpeed;
	
public:	
	UPROPERTY(EditAnywhere)
		AActor* Cube1;
	UPROPERTY(EditAnywhere)
		AActor* Cube2;
	// Sets default values for this actor's properties
	ASteering();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void Seek()
	{
		UMovementComponent* MovementComponent = Cast<UMovementComponent>(Cube1->GetComponentByClass(UFloatingPawnMovement::StaticClass()));
		if(MovementComponent != nullptr)
		{
			FVector normalized = Cube2->GetActorLocation() - Cube1->GetActorLocation();
			normalized.Normalize();
			MovementComponent->Velocity = (normalized * Cube1MaxSpeed) - Cube1->GetVelocity();
		}
	}

};
