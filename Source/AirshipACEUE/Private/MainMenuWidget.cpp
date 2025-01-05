// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	}

	if (AirshipDropdown)
	{
		AirshipDropdown->OnSelectionChanged.AddDynamic(this, &UMainMenuWidget::OnAirshipSelected);
	}

	PopulateAirshipDropdown();
}

void UMainMenuWidget::PopulateAirshipDropdown()
{
    if (!AirshipDropdown)
    {
        UE_LOG(LogTemp, Error, TEXT("AirshipDropdown widget not found! Ensure it is properly bound in the widget blueprint."));
        return;
    }

    AirshipDropdown->ClearOptions();

    // Define the folder path where airship blueprints are stored
    const FString Path = "/Game/Airships"; // Update with the actual folder path
    TArray<FAssetData> AssetDataArray;

    // Access the Asset Registry to find assets
    if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
    {
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.Get().GetAssetsByPath(FName(*Path), AssetDataArray, true);

        // Check if assets were found
        if (AssetDataArray.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("No assets found in folder: %s"), *Path);
        }

    	for (const FAssetData& AssetData : AssetDataArray)
    	{
    		// Check if the asset is of type Blueprint
    		if (AssetData.AssetClassPath.ToString().Contains("Blueprint"))
    		{
    			// Add the asset's name to the dropdown
    			FString AssetName = AssetData.AssetName.ToString();
    			AirshipDropdown->AddOption(AssetName);
    			UE_LOG(LogTemp, Log, TEXT("Added airship: %s"), *AssetName);
    		}
    		else
    		{
    			UE_LOG(LogTemp, Warning, TEXT("Asset %s is not a Blueprint and was skipped."), *AssetData.AssetName.ToString());
    		}
    	}

        // Log the results for debugging
        UE_LOG(LogTemp, Log, TEXT("Found %d airship assets in folder: %s"), AssetDataArray.Num(), *Path);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AssetRegistry module is not loaded! Cannot retrieve airship assets."));
    }

    // Final check if the dropdown is still empty after processing
    if (AirshipDropdown->GetOptionCount() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("AirshipDropdown is empty after attempting to populate. Verify folder contents or path."));
    }
}


void UMainMenuWidget::OnAirshipSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp, Log, TEXT("Selected Airship: %s"), *SelectedItem);
}

void UMainMenuWidget::OnStartButtonClicked()
{
	FString SelectedAirship = AirshipDropdown ? AirshipDropdown->GetSelectedOption() : FString("None");
	UE_LOG(LogTemp, Log, TEXT("Starting with Airship: %s"), *SelectedAirship);
	
	UGameplayStatics::OpenLevel(this, FName("testlevel"));
}

