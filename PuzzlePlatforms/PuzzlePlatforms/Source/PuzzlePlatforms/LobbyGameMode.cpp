// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "PuzzlePlatformsGameInstance.h"

const static int32 MIN_PLAYERS = 2;
const static float START_DELAY = 5;

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= MIN_PLAYERS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached %d players!"), MIN_PLAYERS);

		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, START_DELAY);
	}
}
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	auto PuzzleGameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (PuzzleGameInstance == nullptr) return;

	PuzzleGameInstance->StartSession();


	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}
