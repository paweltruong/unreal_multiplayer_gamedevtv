// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKartMovementComponent.h"
#include "GoKart.generated.h"



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

	UPROPERTY(EditAnywhere)
	UGoKartMovementComponent* MovementComponent;


	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	TArray<FGoKartMove> UnacknowledgedMoves;

	UFUNCTION()
	void OnRep_ServerState();



	FString GetEnumText(ENetRole Role);


	void ClearAcknowledgedMoves(FGoKartMove LastMove);

	void MoveForward(float Value);
	void MoveRight(float Value);


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
