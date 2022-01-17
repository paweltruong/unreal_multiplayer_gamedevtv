// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"

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
		void Host() override;
	UFUNCTION(Exec)
		void Join(const FString& Address) override;


	UFUNCTION(Exec, BlueprintCallable)
		void LoadInGameMenu();

	virtual void LoadMainMenu() override;
	virtual void Quit() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* Menu;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;
	void OnCreateSessionComplete(FName SessionName, bool Success);
};
