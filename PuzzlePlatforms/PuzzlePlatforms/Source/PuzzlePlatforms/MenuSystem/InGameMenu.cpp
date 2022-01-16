// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"



bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!CancelButton) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::OnCancelClicked);

	if (!QuitButton) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitClicked);


	return true;
}

void UInGameMenu::OnCancelClicked()
{
}

void UInGameMenu::OnQuitClicked()
{
}
