// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!HostButton) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);

	if (!JoinButton) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);

	return true;
}

void UMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host button clicked"));

	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OnJoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join button clicked"));
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceImplementation)
{
	this->MenuInterface = MenuInterfaceImplementation;
}

void UMainMenu::Setup()
{
	this->AddToViewport();
	//MainMenu->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();//GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;


	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;

}

void UMainMenu::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();//GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;


	PlayerController->bShowMouseCursor = false;

	FInputModeGameOnly InputModeData;
	/*InputModeData.SetConsumeCaptureMouseDown
		.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);*/

	PlayerController->SetInputMode(InputModeData);
}
