// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "menuSystem/MenuWidget.h"

const static int32 MAX_PLAYERS = 5;
const static FName SESSION_NAME = NAME_GameSession;//TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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


	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class != NULL)
	{
		InGameMenuClass = InGameMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found class %sr"), *InGameMenuBPClass.Class->GetName());
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session interface"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

	if (GEngine!= nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this,&UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (MenuClass != nullptr)
	{
		Menu = CreateWidget<UMainMenu>(this, MenuClass);//CreateWidget<UUserWidget,UWorld>(GetWorld(), MenuClass, FName(TEXT("MainMenu")));
		if (!ensure(Menu != nullptr)) return;

		Menu->Setup();
		Menu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	UE_LOG(LogTemp, Warning, TEXT("Hosting"));

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	//World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}
void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete"));

	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());

			FServerData Data;
			//Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Data found in settings: %s"), *ServerName);
				Data.Name = ServerName;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Didnt get expected data"));
				Data.Name = "Could not find name";
			}
			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}
void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Finding sessions..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;


		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem != nullptr)
		{
			if (Subsystem->GetSubsystemName() == "NULL")
			{
				SessionSettings.bIsLANMatch = true;
			}
			else
			{
				SessionSettings.bIsLANMatch = false;
			}
		}

		SessionSettings.NumPublicConnections = MAX_PLAYERS;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UE_LOG(LogTemp, Warning, TEXT("Joining"));


	if (Menu != nullptr)
	{
		//Menu->SetServerList({"Test1", "Test2"});
		Menu->TearDown();
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Join(const uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Joining by index"));

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass != nullptr)
	{
		UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);//CreateWidget<UUserWidget,UWorld>(GetWorld(), MenuClass, FName(TEXT("MainMenu")));
		if (!ensure(InGameMenu != nullptr)) return;

		InGameMenu->Setup();
		InGameMenu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit to menu"));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/PuzzlePlatforms/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Quit()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit", false);
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not retrieve session connect string"));
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

