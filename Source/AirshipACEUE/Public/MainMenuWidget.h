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
	class UTextBlock* DryMassBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalMassBox;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EngineMassBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyMassChangesButton;

	// Panel that holds all dynamically created weapon selection UI elements
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HardpointListPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyLoadoutButton;
	
	// Mapping: Hardpoint -> Weapon Dropdown
	UPROPERTY(BlueprintReadWrite, Category = "Weapon Selection")
	TMap<UWeaponHardpoint*, class UComboBoxString*> HardpointWeaponDropdowns;

	// Mapping: Weapon -> Projectile Dropdown
	UPROPERTY(BlueprintReadWrite, Category = "Weapon Selection")
	TMap<UWeaponHardpoint*, class UComboBoxString*> HardpointProjectileDropdowns;

	// Mapping: Weapon -> Ammo Input Field
	UPROPERTY(BlueprintReadWrite, Category = "Weapon Selection")
	TMap<UWeaponHardpoint*, class UEditableTextBox*> HardpointAmmoInputs;

	// Populate weapon selection UI dynamically
	UFUNCTION(BlueprintCallable)
	void PopulateWeaponSelectionUI();

	// Handle weapon selection change
	UFUNCTION()
	void OnWeaponSelected(FString SelectedWeapon, ESelectInfo::Type SelectionType);

	// Handle projectile selection change
	UFUNCTION()
	void OnProjectileSelected(FString SelectedProjectile, UWeaponHardpoint* Hardpoint);

	// Handle ammo input change
	UFUNCTION()
	void OnAmmoAmountChanged(const FText& AmmoText, UWeaponHardpoint* Hardpoint);

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

	UFUNCTION()
	void OnApplyLoadoutClicked();

	UFUNCTION()
	bool IsValidInput(const FString& Input);
	
};
