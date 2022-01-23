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
	FVector Velocity;

	/** Handle pressing forwards */
	void MoveForward(float Val);
	///** Setup the strings used on the hud */
	//void SetupInCarHUD();

	///** Update the physics material used by the vehicle mesh */
	//void UpdatePhysicsMaterial();
	///** Handle pressing right */
	//void MoveRight(float Val);
	///** Handle handbrake pressed */
	//void OnHandbrakePressed();
	///** Handle handbrake released */
	//void OnHandbrakeReleased();
};
