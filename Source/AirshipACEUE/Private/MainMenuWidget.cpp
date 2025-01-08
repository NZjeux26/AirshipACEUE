// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "AirGameInstance.h"
#include "Airship.h"
#include "Components/EditableTextBox.h"
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
	
	if (ApplyMassChangesButton)
	{
		ApplyMassChangesButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnApplyMassChangesClicked);
	}

	PopulateAirshipDropdown();
}
//When hitting the apply button, get the masses from the user input boxes, check tey are not < 0 and then set them, update total
void UMainMenuWidget::OnApplyMassChangesClicked()
{
	if (!FuelMassBox || !CargoMassBox || !BallastMassBox || !WeaponsMassBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Mass input fields not bound properly!"));
		return;
	}

	// Read values as strings from input fields
	FString FuelMassStr = FuelMassBox->GetText().ToString();
	FString CargoMassStr = CargoMassBox->GetText().ToString();
	FString BallastMassStr = BallastMassBox->GetText().ToString();
	FString WeaponsMassStr = WeaponsMassBox->GetText().ToString();

	//Checks for valid input, EG no blanks or non-numbers
	if (!IsValidInput(FuelMassStr) || !IsValidInput(CargoMassStr) || !IsValidInput(BallastMassStr) || !IsValidInput(WeaponsMassStr))
	{
		if (ErrorText)
		{
			ErrorText->SetVisibility(ESlateVisibility::Visible);
			ErrorText->SetText(FText::FromString(TEXT("Error Mass values must be valid numbers!")));
		}
		UE_LOG(LogTemp, Warning, TEXT("All mass values must be valid numbers!"));
		return;
	}
	//convert to floats
	float FuelMass = FCString::Atof(*FuelMassStr);
	float BallastMass = FCString::Atof(*BallastMassStr);
	float CargoMass = FCString::Atof(*CargoMassStr);
	float WeaponMass = FCString::Atof(*WeaponsMassStr);
	
	//check they are not negative numbers
	if (FuelMass < 0 || CargoMass < 0 || BallastMass < 0 || WeaponMass < 0)
	{
		if (ErrorText)
		{
			ErrorText->SetVisibility(ESlateVisibility::Visible);
			ErrorText->SetText(FText::FromString(TEXT("Error Mass values must be > 0!")));
		}
		//ToAdd later, abilty to check for numbers only, and check whether the total mass is > than the MTOW.
		UE_LOG(LogTemp, Warning, TEXT("Mass values must be non-negative!"));
		return;
	}
	
	if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (GI->SelectedAirship)
		{
			if (ErrorText)	ErrorText->SetVisibility(ESlateVisibility::Hidden);	
			
			GI->FuelMass = FuelMass;
			GI->CargoMass = CargoMass;
			GI->BallastMass = BallastMass;
			GI->WeaponMass = WeaponMass;

			// Retrieve DryMass from the default object of the selected airship class
			AAirship* TempAirship = GI->SelectedAirship->GetDefaultObject<AAirship>();
			float DryMass = TempAirship ? TempAirship->GetDryMass() : 0.0f;

			// Calculate TotalMass including DryMass
			float TotalMass = DryMass + FuelMass + CargoMass + BallastMass + WeaponMass + 8;
			//float TotalMass = FuelMass + CargoMass + BallastMass + 8;
			TotalMassBox->SetText(FText::AsNumber(TotalMass));
			UE_LOG(LogTemp, Log, TEXT("Updated airship masses: Fuel=%f, Cargo=%f, Ballast=%f, Weapons=%f"),
			FuelMass, CargoMass, BallastMass, WeaponMass);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No airship selected. Cannot apply masses."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance."));
	}
}

void UMainMenuWidget::PopulateMassFields()
{
	if (!FuelMassBox || !CargoMassBox || !BallastMassBox || !WeaponsMassBox ||!EngineMassBox)
	{
		UE_LOG(LogTemp, Error, TEXT("Mass input fields not bound properly!"));
		return;
	}

	// Get the selected airship class from the GameInstance
	if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (GI->SelectedAirship)
		{
			// Create a temporary airship actor to access its default properties
			AAirship* TempAirship = GI->SelectedAirship->GetDefaultObject<AAirship>();
			if (TempAirship)
			{
				// Populate the input fields with current masses
				FuelMassBox->SetText(FText::AsNumber(TempAirship->GetFuelMass()));
				CargoMassBox->SetText(FText::AsNumber(TempAirship->GetCargoMass()));
				BallastMassBox->SetText(FText::AsNumber(TempAirship->GetBallastMass()));
				WeaponsMassBox->SetText(FText::AsNumber(TempAirship->GetWeaponsMass()));
				DryMassBox->SetText(FText::AsNumber(TempAirship->GetDryMass()));
				EngineMassBox->SetText(FText::AsNumber(8));//Fixed to 8 for now until I can work a way to get the engine mass
				TempAirship->UpdateTotalMass();
				TotalMassBox->SetText(FText::AsNumber(TempAirship->GetTotalMass()));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to retrieve default object of selected airship."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No airship selected to populate masses."));
		}
	}
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

//when an airship is selected in the dropdown, it's assigned to the gameinstance as the airship, its mass values are called, and populate the input boxes
void UMainMenuWidget::OnAirshipSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp, Log, TEXT("Selected Airship: %s"), *SelectedItem);

	// Hide the error message when the user selects an airship
	if (ErrorText)
	{
		ErrorText->SetVisibility(ESlateVisibility::Hidden);
	}

	// Store the selected airship in the GameInstance
	if (!SelectedItem.IsEmpty())
	{
		FString Path = "/Game/Airships/" + SelectedItem + "." + SelectedItem + "_C";
		UClass* AirshipClass = LoadClass<APawn>(nullptr, *Path);

		if (AirshipClass)
		{
			if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				GI->SelectedAirship = AirshipClass;
				UE_LOG(LogTemp, Log, TEXT("Selected Airship stored in GameInstance: %s"), *SelectedItem);

				//Populate mass fields for the selected airship
				PopulateMassFields();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No airship selected."));
	}
}
//when the start button is clicked, whatever is the assigned airship in the gameinstance is passed to the level
void UMainMenuWidget::OnStartButtonClicked()
{
	if (!AirshipDropdown)
	{
		UE_LOG(LogTemp, Error, TEXT("AirshipDropdown widget not found!"));
		return;
	}

	// Get the GameInstance and validate the selection
	if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (!GI->SelectedAirship)
		{
			// If no airship is selected, show an error
			if (ErrorText)
			{
				ErrorText->SetText(FText::FromString(TEXT("Please select an airship before starting!")));
				ErrorText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ErrorText widget not found!"));
			}

			return; // Do not proceed
		}

		// Log the selected airship
		UE_LOG(LogTemp, Log, TEXT("Starting game with selected airship."));
	}

	// Hide the error message before loading the level
	if (ErrorText)
	{
		ErrorText->SetVisibility(ESlateVisibility::Hidden);
	}

	// Proceed to load the level
	UGameplayStatics::OpenLevel(this, FName("testlevel"));
}

bool UMainMenuWidget::IsValidInput(const FString& Input)
{
	return !Input.IsEmpty() && Input.IsNumeric();
}

