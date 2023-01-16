// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

UCLASS(Blueprintable)
class PROJET_STEERING_API AVehicle : public APawn
{
	GENERATED_BODY()
	bool Paused;

public:
	FVector Velocity ;
	
	UPROPERTY(EditAnywhere)
	float Mass;
	
	UPROPERTY(EditAnywhere)
	float MaxForce ;
	
	UPROPERTY(EditAnywhere)
	float MaxSpeed ;
	
	// Sets default values for this pawn's properties
	AVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ChangePaused()
	{
		Paused = !Paused;
	}
};
