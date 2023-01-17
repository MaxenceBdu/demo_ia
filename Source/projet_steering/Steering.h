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
	FString Mode;
	float Mass, MaxSpeed, MaxForce;
	typedef FVector (ASteering::*FunctionPtrType)();
	FunctionPtrType MovementFunction;
public:
	UPROPERTY(EditAnywhere)
		AActor* Target;
	UPROPERTY(EditAnywhere)
		AVehicle* Vehicle;
	UPROPERTY(EditAnywhere)
		bool Paused;
	UPROPERTY(EditAnywhere)
		FVector Velocity;


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

	UFUNCTION(BlueprintCallable)
	void SetCurrentMode(FString NewMode)
	{
		Mode = NewMode;
		if(Mode.Equals("Seek"))
			MovementFunction = &ASteering::Seek;
		else if(Mode.Equals("Flee"))
			MovementFunction = &ASteering::Flee;
		else
		{
			MovementFunction = &ASteering::Seek;
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
private:
	
	FVector Seek()
	{
		if(Target->GetActorLocation() != Vehicle->GetActorLocation())
		{
			FVector Normalized = Target->GetActorLocation() - Vehicle->GetActorLocation();
			Normalized.Normalize();
			return Normalized * MaxSpeed - Velocity;
		}
		return FVector(0,0,0);
	}

	FVector Flee()
	{
		return Seek()*-1;
	}

	FVector Pursuit()
	{
		return Velocity;
	}

	FVector Evade()
	{
		return Velocity;
	}
};
