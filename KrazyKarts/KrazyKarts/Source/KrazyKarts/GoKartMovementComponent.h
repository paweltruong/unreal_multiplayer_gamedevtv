// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"



USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;
	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FGoKartMove CreateMove(float DeltaTime);
	void SimulateMove(const FGoKartMove Move);

	FVector GetVelocity() { return Velocity; }
	void SetVelocity(FVector Val) { Velocity = Val; }
	void SetThrottle(float Val) { Throttle = Val; }
	void SetSteeringThrow(float Val) { SteeringThrow = Val; }
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
	/// Minimum radius of the car turning circle at full lock (m) https://en.wikipedia.org/wiki/Turning_radius
	/// </summary>
	UPROPERTY(EditAnywhere)
	float MinimumTurningRadius = 10;
	/// <summary>
	/// Higher means more drag
	/// </summary>
	UPROPERTY(EditAnywhere)
	float DragCoefficiency = 16;
	/// <summary>
	/// Higher means more rolling resistance https://en.wikipedia.org/wiki/Rolling_resistance
	/// </summary>
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficiency = 0.015;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;


	/// <summary>
	/// Calculate air resistance
	/// </summary>
	/// <returns></returns>
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void ApplyRotation(float DeltaTime, float InSteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);
		
};
