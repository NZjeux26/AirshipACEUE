// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ReplayButton) ReplayButton->OnClicked.AddDynamic(this, &UGameEndWidget::OnReplayButtonClicked);
	
	if (MainMenuButton) MainMenuButton->OnClicked.AddDynamic(this, &UGameEndWidget::OnMainMenuButtonClicked);
	//StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
}

void UGameEndWidget::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenuLevel"));
}

void UGameEndWidget::OnReplayButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("testlevel"));
}
