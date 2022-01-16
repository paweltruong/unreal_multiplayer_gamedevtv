// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterfaceImplementation)
{
	this->MenuInterface = MenuInterfaceImplementation;
}

void UMenuWidget::Setup()
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

void UMenuWidget::TearDown()
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