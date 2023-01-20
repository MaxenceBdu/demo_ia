// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "GameFramework/Pawn.h"
#include "Templates/SharedPointer.h"
#include "Vehicle.generated.h"

UCLASS(Blueprintable)
class PROJET_STEERING_API AVehicle : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	ATarget* Target;
	FVector (AVehicle::*MovementFunction)() = nullptr;
	UPROPERTY(VisibleAnywhere)
	float Mass;
	UPROPERTY(VisibleAnywhere)
	float MaxSpeed;
	UPROPERTY(VisibleAnywhere)
	float MaxForce;
	FVector Velocity, Zero = FVector::Zero();
	bool Arrived;
	

public:	
	// Sets default values for this pawn's properties
	AVehicle();

	UFUNCTION(BlueprintCallable)
	void SetTarget(ATarget* NewTarget)
	{
		Target = NewTarget;
	}

	UFUNCTION(BlueprintCallable)
	void SetMovementFunction(int Id)
	{
		switch (Id)
		{
		case 0:
			MovementFunction = &AVehicle::Seek;
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1,15.0, FColor::Red, TEXT("aled"));
			break;
		case 1:
			MovementFunction = &AVehicle::Flee;
			break;
		case 2:
			MovementFunction = &AVehicle::Pursuit;
			break;
		case 3:
			MovementFunction = &AVehicle::Evade;
			break;
		case 4:
			MovementFunction = &AVehicle::Arrival;
			break;
		default:
			break;
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetMass(float NewMass)
	{
		Mass = NewMass;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetMaxSpeed(float NewMaxSpeed)
	{
		MaxSpeed = NewMaxSpeed;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetMaxForce(float NewMaxForce)
	{
		MaxForce = NewMaxForce;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector Seek();

	virtual FVector Flee();

	virtual FVector Pursuit();

	virtual FVector Evade();

	virtual FVector Arrival();
};
