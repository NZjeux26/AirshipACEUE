// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "AirGameInstance.h"
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
	//Hide the error message when the user selects an airship if the popup is visible.
	if (ErrorText)
	{
		// Hide the error message whenever a selection is made
		ErrorText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainMenuWidget::OnStartButtonClicked()
{
	//if airshipdropdown is present, then run the below.
	if (AirshipDropdown)
	{
		// Get the selected option from the dropdown
		FString SelectedAirshipName = AirshipDropdown->GetSelectedOption();

		// If no airship is selected, show an error
		if (SelectedAirshipName.IsEmpty())
		{
			// Ensure ErrorText exists before using it
			if (ErrorText)
			{
				// Display an error message on the screen
				ErrorText->SetText(FText::FromString(TEXT("Please select an airship before starting!")));
				ErrorText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ErrorText widget not found!"));
			}

			// Do not proceed to level loading
			return;
		}

		// Hide the error message if the selection is valid
		if (ErrorText)
		{
			ErrorText->SetVisibility(ESlateVisibility::Hidden);
		}
		
		//FString SelectedAirshipName = AirshipDropdown->GetSelectedOption();
		if (!SelectedAirshipName.IsEmpty())
		{
			// Construct the path to the selected airship blueprint
			FString Path = "/Game/Airships/" + SelectedAirshipName + "." + SelectedAirshipName + "_C";
			UClass* AirshipClass = LoadClass<APawn>(nullptr, *Path);

			if (AirshipClass)
			{
				// Store the selected airship in the GameInstance
				if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					GI->SelectedAirship = AirshipClass;
					UE_LOG(LogTemp, Log, TEXT("Selected Airship: %s"), *SelectedAirshipName);
				}
			}
		}
	}
	//if no dropdown found, exit
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AirshipDropdown widget not found!"));
		return;
	}
	
	UGameplayStatics::OpenLevel(this, FName("testlevel"));
}

