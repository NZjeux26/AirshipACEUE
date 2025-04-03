// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "AirGameInstance.h"
#include "Airship.h"
#include "Blueprint/WidgetTree.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
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

	if (ApplyLoadoutButton)
	{
		ApplyLoadoutButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnApplyLoadoutClicked);
	}

	PopulateAirshipDropdown();
}

void UMainMenuWidget::PopulateWeaponSelectionUI()
{
	UE_LOG(LogTemp, Log, TEXT("Populating weapon selection UI..."));

	// Ensure the panel is properly set up
	if (!HardpointListPanel)
	{
		UE_LOG(LogTemp, Error, TEXT("HardpointListPanel is not bound to the widget."));
		return;
	}

	// Clear previous UI elements
	HardpointListPanel->ClearChildren();
	HardpointWeaponDropdowns.Empty();
	HardpointProjectileDropdowns.Empty();
	HardpointAmmoInputs.Empty();

	// Get the selected airship from GameInstance
	if (UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		// Ensure hardpoints have been initialized
		if (GI->AirshipLoadout.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No hardpoints found in GameInstance. Initializing..."));
			GI->InitializeHardpointsFromAirship();
		}

		UE_LOG(LogTemp, Log, TEXT("Populating UI for %d hardpoints."), GI->AirshipLoadout.Num());

		// Iterate through the available hardpoints
		for (FHardpointLoadout& Loadout : GI->AirshipLoadout)
		{
			UE_LOG(LogTemp, Log, TEXT("Adding UI for hardpoint: %s"), *Loadout.HardpointName);

			// Ensure the hardpoint reference is valid
			if (!Loadout.Hardpoint)
			{
				UE_LOG(LogTemp, Warning, TEXT("Loadout has no valid Hardpoint! Check if InitializeHardpointsFromAirship() was called."));
				continue;
			}

			//Create a vertical box to contain the label and dropdown
			UVerticalBox* HardpointContainer = WidgetTree->ConstructWidget<UVerticalBox>();
			HardpointListPanel->AddChild(HardpointContainer);
			
			//Create a Label for the drop down
			UTextBlock* ProjectileLabel = WidgetTree->ConstructWidget<UTextBlock>();
			ProjectileLabel->SetText(FText::FromString(Loadout.HardpointName));
			HardpointContainer->AddChild(ProjectileLabel);
			
			// **Weapon Dropdown**
			UComboBoxString* WeaponDropdown = WidgetTree->ConstructWidget<UComboBoxString>();
			HardpointContainer->AddChild(WeaponDropdown);
			
			// Add weapon options dynamically using AssetRegistry
			FString WeaponsPath = "/Game/Weapons";
			TArray<FAssetData> WeaponAssets;
			FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			AssetRegistry.Get().GetAssetsByPath(FName(*WeaponsPath), WeaponAssets, true);

			for (const FAssetData& Asset : WeaponAssets)
			{
				// Ensure it's a valid weapon blueprint
				if (UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset()))
				{
					if (Blueprint->GeneratedClass && Blueprint->GeneratedClass->IsChildOf(AWeapon::StaticClass()))
					{
						WeaponDropdown->AddOption(Asset.AssetName.ToString());
					}
				}
			}

			// Bind selection event and Call OnWeaponSelected to assign the weapon to the airship hardpoint
			WeaponDropdown->OnSelectionChanged.AddDynamic(this, &UMainMenuWidget::OnWeaponSelected);
			// Store reference for later use
			HardpointWeaponDropdowns.Add(Loadout.HardpointName, WeaponDropdown);
			HardpointContainers.Add(Loadout.HardpointName, HardpointContainer);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve GameInstance."));
	}
}

void UMainMenuWidget::PopulateProjSelectionUI(const FString& HardpointName)
{
	UVerticalBox* HardpointContainer = HardpointContainers.FindRef(HardpointName);
	if (!HardpointContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find container for hardpoint: %s"), *HardpointName);
		return;
	}
	// Create a label for the projectile dropdown
	UTextBlock* ProjectileLabel = WidgetTree->ConstructWidget<UTextBlock>();
	ProjectileLabel->SetText(FText::FromString(TEXT("Projectile:")));
	HardpointContainer->AddChild(ProjectileLabel);
	
	//create the dropdown
	UComboBoxString* ProjectileDropdown = WidgetTree->ConstructWidget<UComboBoxString>();
	HardpointContainer->AddChild(ProjectileDropdown);
	
	UE_LOG(LogTemp, Log, TEXT("Created projectile dropdown for hardpoint: %s"), *HardpointName);
	
	//Add projectiles options
	FString ProjectilesPath = "/Game/Weapons/Projectiles";
	TArray<FAssetData> ProjectileAssets;
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistry.Get().GetAssetsByPath(FName(*ProjectilesPath), ProjectileAssets, true);

	for (const FAssetData& Asset : ProjectileAssets)
	{
		// Ensure it's a valid weapon blueprint
		if (UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset()))
		{
			if (Blueprint->GeneratedClass && Blueprint->GeneratedClass->IsChildOf(AProjectile::StaticClass()))
			{
				ProjectileDropdown->AddOption(Asset.AssetName.ToString());
				UE_LOG(LogTemp, Log, TEXT("Added projectile option: %s"), *Asset.AssetName.ToString());
			}
		}
	}
	//bind and call OnProjectileSelected to bind the projectile to the WeaponBP selected
	ProjectileDropdown->OnSelectionChanged.AddDynamic(this,&UMainMenuWidget::OnProjectileSelected);
	// Store reference for later use
	HardpointProjectileDropdowns.Add(HardpointName, ProjectileDropdown);
	UE_LOG(LogTemp, Log, TEXT("Added projectile dropdown to map with key: %s"), *HardpointName);
}
//Checks selection was human, finds the hardpoint selected and exstracts the name of it
void UMainMenuWidget::OnWeaponSelected(FString SelectedWeapon, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Type::Direct) return; // Ignore automatic selections

	UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
		return;
	}

	// Find the hardpoint name associated with the dropdown that fired this event
	FString HardpointName;
	for (const auto& Pair : HardpointWeaponDropdowns)
	{
		if (Pair.Value && Pair.Value->GetSelectedOption() == SelectedWeapon)
		{
			HardpointName = Pair.Key;
			break;
		}
	}

	if (HardpointName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find the matching hardpoint for weapon: %s"), *SelectedWeapon);
		return;
	}
	PopulateProjSelectionUI(HardpointName);
}

void UMainMenuWidget::OnProjectileSelected(FString SelectedProjectile, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Type::Direct) return; // Ignore automatic selections

	UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
		return;
	}

	// Find the hardpoint name associated with the dropdown that fired this event
	FString HardpointName;
	for (const auto& Pair : HardpointProjectileDropdowns)
	{
		if (Pair.Value && Pair.Value->GetSelectedOption() == SelectedProjectile)
		{
			HardpointName = Pair.Key;
			break;
		}
	}

	if (HardpointName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find the matching hardpoint for projectile: %s"), *SelectedProjectile);
		return;
	}

	// Find the correct Hardpoint Loadout in GameInstance
	for (FHardpointLoadout& Loadout : GI->AirshipLoadout)
	{
		if (Loadout.HardpointName == HardpointName)
		{
			// Load the projectile class dynamically
			FString Path = "/Game/Weapons/Projectiles/" + SelectedProjectile + "." + SelectedProjectile + "_C";
			Loadout.SelectedProjectile = LoadClass<AProjectile>(nullptr, *Path);

			// Try to load the class directly
			UClass* ProjectileClass = StaticLoadClass(AProjectile::StaticClass(), nullptr, *Path);
			if (ProjectileClass)
			{
				// Cast to ensure it's the right type
				Loadout.SelectedProjectile = TSubclassOf<AProjectile>(ProjectileClass);
				UE_LOG(LogTemp, Log, TEXT("Projectile %s assigned to hardpoint %s"), *SelectedProjectile, *HardpointName);
			}
			else
			{
				// Try loading as a blueprint
				FString BlueprintPath = "/Game/Weapons/Projectiles/" + SelectedProjectile + "." + SelectedProjectile;
				UBlueprint* ProjectileBlueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
                
				if (ProjectileBlueprint && ProjectileBlueprint->GeneratedClass)
				{
					Loadout.SelectedProjectile = TSubclassOf<AProjectile>(ProjectileBlueprint->GeneratedClass);
					UE_LOG(LogTemp, Log, TEXT("Projectile %s assigned to hardpoint %s via blueprint"), 
						   *SelectedProjectile, *HardpointName);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to load projectile class for %s from path %s"), 
						   *SelectedProjectile, *Path);
				}
			}
			// You could set default ammo count here if needed
			// Loadout.AmmoCount = 100;  

			UE_LOG(LogTemp, Log, TEXT("Projectile %s assigned to hardpoint %s"), *SelectedProjectile, *HardpointName);
			break;
		}
	}

	//code for ammo stuff.
}

void UMainMenuWidget::OnAmmoAmountChanged(const FText& AmmoText, UWeaponHardpoint* Hardpoint)
{
	if (!Hardpoint) return;

	int32 AmmoAmount = FCString::Atoi(*AmmoText.ToString());

	AWeapon* WeaponInstance = Hardpoint->WeaponToMount->GetDefaultObject<AWeapon>();
	if (WeaponInstance)
	{
		WeaponInstance->SetAmmo(AmmoAmount);
	}
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

    // Access the Asset Registry to find assets
    if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
    {
	    TArray<FAssetData> AssetDataArray; //If it breaks it was because of this <<<<<<<<
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

				// Initialize hardpoint loadout
				GI->InitializeHardpointsFromAirship();
				
				//Populate mass fields for the selected airship
				PopulateMassFields();
				PopulateWeaponSelectionUI();
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
	UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
		return;
	}
	
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
	//check if each hardpoint has a weapon assigned to it
	bool hasWeapons = false;
	for (const FHardpointLoadout& Loadout: GI->AirshipLoadout)
	{
		if (Loadout.SelectedWeapon)
		{
			hasWeapons = true;
			break;
		}
	}

	if (!hasWeapons)
	{
		if (ErrorText)
		{
			ErrorText->SetText(FText::FromString(TEXT("Please assign at least one weapon before starting!")));
			ErrorText->SetVisibility(ESlateVisibility::Visible);
		}
		else UE_LOG(LogTemp, Error, TEXT("ErrorText widget not found!"));

		return;
	}
	// Log the selected airship
	UE_LOG(LogTemp, Log, TEXT("Starting game with selected airship."));

	// Hide the error message before loading the level
	if (ErrorText)
	{
		ErrorText->SetVisibility(ESlateVisibility::Hidden);
	}

	// Proceed to load the level
	UGameplayStatics::OpenLevel(this, FName("testlevel"));
}

void UMainMenuWidget::OnApplyLoadoutClicked()
{
	 UE_LOG(LogTemp, Log, TEXT("Applying weapon loadout..."));

    // Get GameInstance
    UAirGameInstance* GI = Cast<UAirGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GI)
    {
       UE_LOG(LogTemp, Error, TEXT("GameInstance not found!"));
       return;
    }

    // Check if there are hardpoints available
    if (GI->AirshipLoadout.Num() == 0)
    {
       UE_LOG(LogTemp, Warning, TEXT("No hardpoints found in GameInstance!"));
       return;
    }

    // Update all hardpoints with their selected weapons and projectiles
    for (FHardpointLoadout& Loadout : GI->AirshipLoadout)
    {
        FString HardpointName = Loadout.HardpointName;
        
        // Get the weapon dropdown for this hardpoint
        UComboBoxString* WeaponDropdown = HardpointWeaponDropdowns.FindRef(HardpointName);
        if (!WeaponDropdown)
        {
            UE_LOG(LogTemp, Warning, TEXT("Weapon dropdown not found for hardpoint %s"), *HardpointName);
            continue;
        }
        
        // Get the selected weapon name
        FString SelectedWeapon = WeaponDropdown->GetSelectedOption();
        if (SelectedWeapon.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("No weapon selected for hardpoint %s"), *HardpointName);
            continue;
        }
        
        // Update loadout with selected weapon
        Loadout.WeaponName = SelectedWeapon;
        
        // Load the weapon class
        FString WeaponPath = "/Game/Weapons/" + SelectedWeapon + "." + SelectedWeapon + "_C";
        Loadout.SelectedWeapon = LoadClass<AWeapon>(nullptr, *WeaponPath);

    	if (!Loadout.SelectedWeapon)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Failed to load weapon class for %s from path %s"), *SelectedWeapon, *WeaponPath);
    	}

    	// Get the projectile dropdown for this hardpoint
    	UComboBoxString* ProjectileDropdown = HardpointProjectileDropdowns.FindRef(HardpointName);
    	if (!ProjectileDropdown)
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Projectile dropdown not found for hardpoint %s"), *HardpointName);
    		continue;
    	}

    	FString SelectedProjectile = ProjectileDropdown->GetSelectedOption();
    	if (SelectedProjectile.IsEmpty())
    	{
    		UE_LOG(LogTemp, Warning, TEXT("No projectile selected for weapon %s"), *SelectedWeapon);
    		continue;
    	}

    	FString ProjectilePath = "/Game/Weapons/Projectiles";
    	Loadout.SelectedProjectile = LoadClass<AProjectile>(nullptr, *SelectedProjectile);
    	if (!Loadout.SelectedProjectile)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Failed to load projectile class for %s from path %s"), *SelectedProjectile, *ProjectilePath);
    	}
    	
        UE_LOG(LogTemp, Log, TEXT("Weapon %s assigned to hardpoint %s"), *SelectedWeapon, *HardpointName);
    	UE_LOG(LogTemp,Log, TEXT("Projectile %s assigned to weapon %s"),*SelectedProjectile,*SelectedWeapon);
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon loadout successfully applied."));
}

bool UMainMenuWidget::IsValidInput(const FString& Input)
{
	return !Input.IsEmpty() && Input.IsNumeric();
}

