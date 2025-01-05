// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AirGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AIRSHIPACEUE_API UAirGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// Stores the selected airship blueprint class
	UPROPERTY(BlueprintReadWrite, Category = "Airship")
	TSubclassOf<class APawn> SelectedAirship;
};
