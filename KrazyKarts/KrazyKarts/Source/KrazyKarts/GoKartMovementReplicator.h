// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"


USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class AGoKart* Pawn;
	TArray<FGoKartMove> UnacknowledgedMoves;
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UPROPERTY()
	UGoKartMovementComponent* MovementComponent;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();

	void ClearAcknowledgedMoves(FGoKartMove LastMove);

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
		
	void UpdateServerState(const FGoKartMove& Move);

	void ClientTick(float DeltaTime);
};
