// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	/// <summary>
	/// The mass of the car (kg)
	/// </summary>
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	/// <summary>
	/// Force applied to the car when the throttle is fully down (N)
	/// </summary>
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	/// <summary>
	/// The number of degrees rotated per second at full controll throw (degrees/s)
	/// </summary>
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);

	/// <summary>
	/// Handle pressing forwards
	/// </summary>
	/// <param name="Val"></param>
	void MoveForward(float Value);
	/// <summary>
	/// Handle pressing right
	/// </summary>
	/// <param name="Val"></param>
	void MoveRight(float Value);

	///** Setup the strings used on the hud */
	//void SetupInCarHUD();

	///** Update the physics material used by the vehicle mesh */
	//void UpdatePhysicsMaterial();
	///** Handle handbrake pressed */
	//void OnHandbrakePressed();
	///** Handle handbrake released */
	//void OnHandbrakeReleased();
};
