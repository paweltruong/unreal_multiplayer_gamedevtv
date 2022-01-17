// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuWidget.h"

#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IpAddressField;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;
	TSubclassOf<class UUserWidget> ServerRowClass;

	UFUNCTION()
		void OnExitClicked();
	UFUNCTION()
		void OnHostClicked();
	UFUNCTION()
		void OnJoinClicked();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void OpenMainMenu();

	TOptional<uint32> SelectedIndex;
	void UpdateChildren();
};
