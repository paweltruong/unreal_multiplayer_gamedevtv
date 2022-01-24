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


	UPROPERTY(ReplicatedUsing= OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;

	UFUNCTION()
	void OnRep_ReplicatedTransform();

	/// <summary>
	/// Calculate air resistance
	/// </summary>
	/// <returns></returns>
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);


	FString GetEnumText(ENetRole Role);


	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	/// <summary>
	/// Handle pressing forwards
	/// </summary>
	/// <param name="Val"></param>
	void Server_MoveForward(float Value);
	void Server_MoveForward_Implementation(float Value);
	bool Server_MoveForward_Validate(float Value);
	//LNK2001	unresolved external symbol "public: virtual void __cdecl AGoKart::Server_MoveForward_Implementation(float)" (? Server_MoveForward_Implementation@AGoKart@@UEAAXM@Z)	KrazyKarts	C : \Repos\unreal_multiplayer_gamedevtv\unreal_multiplayer_gamedevtv\KrazyKarts\KrazyKarts\Intermediate\ProjectFiles\GoKart.cpp.obj	1
	//LNK2001	unresolved external symbol "public: virtual bool __cdecl AGoKart::Server_MoveForward_Validate(float)" (? Server_MoveForward_Validate@AGoKart@@UEAA_NM@Z)	KrazyKarts	C : \Repos\unreal_multiplayer_gamedevtv\unreal_multiplayer_gamedevtv\KrazyKarts\KrazyKarts\Intermediate\ProjectFiles\GoKart.gen.cpp.obj	1



	UFUNCTION(Server, Reliable, WithValidation)
	/// <summary>
	/// Handle pressing right
	/// </summary>
	/// <param name="Val"></param>
	void Server_MoveRight(float Value);
	void Server_MoveRight_Implementation(float Value);
	bool Server_MoveRight_Validate(float Value);
};
