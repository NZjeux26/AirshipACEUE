// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class AIRSHIPACEUE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	public:
	//Assignable Start button to start the game level
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* AirshipDropdown;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ErrorText;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* FuelMassBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* CargoMassBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* BallastMassBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* WeaponsMassBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyMassChangesButton;

	// Function to handle Apply button click
	UFUNCTION()
	void OnApplyMassChangesClicked();

	UFUNCTION()
	void PopulateMassFields();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PopulateAirshipDropdown();

	// Function to handle dropdown selection changes
	UFUNCTION()
	void OnAirshipSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	// Function to handle Start Button click
	UFUNCTION()
	void OnStartButtonClicked();
	
};
