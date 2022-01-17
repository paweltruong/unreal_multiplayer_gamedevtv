// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Class != NULL)
	{
		ServerRowClass = ServerRowBPClass.Class;

		UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *ServerRowBPClass.Class->GetName());
	}

}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;

		ServerRow->ServerName->SetText(FText::FromString(ServerName));

		ServerList->AddChild(ServerRow);
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ExitButton) return false;
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitClicked);

	if (!HostButton) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);

	if (!JoinButton) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);

	if (!JoinMenuButton) return false;
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!MainMenuButton) return false;
	MainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::OnExitClicked()
{
	if (MenuInterface)
	{
		MenuInterface->Quit();
	}
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

	if (MenuInterface && IpAddressField)
	{
		//MenuInterface->Join(IpAddressField->GetText().ToString());
		MenuInterface->Join("");
	}
}

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinMenu button clicked"));
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenu button clicked"));
	if (!MenuSwitcher) return;
	if (!MainMenu) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

