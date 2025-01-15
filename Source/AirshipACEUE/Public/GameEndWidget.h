// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class AIRSHIPACEUE_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ReplayButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UFUNCTION()
	void OnMainMenuButtonClicked();
	
	UFUNCTION()
	void OnReplayButtonClicked();
};
