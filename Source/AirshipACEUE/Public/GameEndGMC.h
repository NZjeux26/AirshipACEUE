// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameEndGMC.generated.h"

/**
 * 
 */
UCLASS()
class AIRSHIPACEUE_API AGameEndGMC : public AGameMode
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	// Blueprint-assignable UI Widget Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> UIWidgetClass;
};
