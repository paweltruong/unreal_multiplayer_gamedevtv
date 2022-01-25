// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"


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

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FGoKartMove LastMove;


};

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


	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;
	TArray<FGoKartMove> UnacknowledgedMoves;

	UFUNCTION()
	void OnRep_ServerState();

	/// <summary>
	/// Calculate air resistance
	/// </summary>
	/// <returns></returns>
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void ApplyRotation(float DeltaTime, float InSteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);


	FString GetEnumText(ENetRole Role);


	FGoKartMove CreateMove(float DeltaTime);
	void ClearAcknowledgedMoves(FGoKartMove LastMove);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SimulateMove(const FGoKartMove Move);

	UFUNCTION(Server, Reliable, WithValidation)
	/// <summary>
	/// Handle pressing forwards
	/// </summary>
	/// <param name="Val"></param>
	void Server_SendMove(FGoKartMove Move);
	void Server_SendMove_Implementation(FGoKartMove Move);
	bool Server_SendMove_Validate(FGoKartMove Move);
	//LNK2001	unresolved external symbol "public: virtual void __cdecl AGoKart::Server_MoveForward_Implementation(float)" (? Server_MoveForward_Implementation@AGoKart@@UEAAXM@Z)	KrazyKarts	C : \Repos\unreal_multiplayer_gamedevtv\unreal_multiplayer_gamedevtv\KrazyKarts\KrazyKarts\Intermediate\ProjectFiles\GoKart.cpp.obj	1
	//LNK2001	unresolved external symbol "public: virtual bool __cdecl AGoKart::Server_MoveForward_Validate(float)" (? Server_MoveForward_Validate@AGoKart@@UEAA_NM@Z)	KrazyKarts	C : \Repos\unreal_multiplayer_gamedevtv\unreal_multiplayer_gamedevtv\KrazyKarts\KrazyKarts\Intermediate\ProjectFiles\GoKart.gen.cpp.obj	1
};
