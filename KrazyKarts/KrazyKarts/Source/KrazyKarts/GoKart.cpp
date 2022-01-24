// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"

// Sets default values
AGoKart::AGoKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);

	UpdateLocationFromVelocity(DeltaTime);
}

FVector AGoKart::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficiency;
}

FVector AGoKart::GetRollingResistance()
{
	//UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), GetWorld()->GetGravityZ());//-980.0
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficiency * NormalForce;
}

void AGoKart::ApplyRotation(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * SteeringThrow * DeltaTime;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult Hit;
	AddActorWorldOffset(Translation, true, OUT & Hit);
	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
	//PlayerInputComponent->BindAxis("LookUp");
	//PlayerInputComponent->BindAxis("LookRight");

	//PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AGoKart::OnHandbrakePressed);
	//PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AGoKart::OnHandbrakeReleased);
	//PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AGoKart::OnToggleCamera);

}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}
void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}

