// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_MainMenu"));
	if (MenuBPClass.Class != NULL)
	{
		MenuClass = MenuBPClass.Class;
		//DefaultPawnClass = PlatformTriggerBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found class %sr"), *MenuBPClass.Class->GetName());
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (MenuClass != nullptr)
	{
		UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);//CreateWidget<UUserWidget,UWorld>(GetWorld(), MenuClass, FName(TEXT("MainMenu")));
		if (!ensure(Menu != nullptr)) return;
		Menu->AddToViewport();
		//MainMenu->bIsFocusable = true;


		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr)) return;


		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(Menu->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);

		PlayerController->bShowMouseCursor = true;

		Menu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting"));

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0,2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UE_LOG(LogTemp, Warning, TEXT("Joining"));

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

}
