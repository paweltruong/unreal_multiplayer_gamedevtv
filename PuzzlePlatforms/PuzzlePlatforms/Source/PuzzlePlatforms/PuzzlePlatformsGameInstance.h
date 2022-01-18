// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(Exec,BlueprintCallable)
		void LoadMenu();
	UFUNCTION(Exec)
		void Host(FString ServerName) override;
	UFUNCTION(Exec)
		void Join(const FString& Address) override;


	UFUNCTION(Exec, BlueprintCallable)
		void LoadInGameMenu();

	virtual void LoadMainMenu() override;
	virtual void Quit() override;
	virtual void RefreshServerList() override;
	virtual void Join(const uint32 Index) override;


	/// <summary>
	/// Locks game from joining, session is in progress
	/// </summary>
	void StartSession();

private:
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* Menu;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void CreateSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

};
