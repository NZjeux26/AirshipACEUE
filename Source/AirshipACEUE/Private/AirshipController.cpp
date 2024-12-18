// Fill out your copyright notice in the Description page of Project Settings.


#include "AirshipController.h"
#include "Airship.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"

void AAirshipController::BeginPlay()
{
	Super::BeginPlay();

	SetControlledAirship();
}

void AAirshipController::SetupInputComponent()
{
	Super::SetupInputComponent();
	 if (!InputComponent)
	 {
	 	UE_LOG(LogTemp, Error, TEXT("InputComponent not initialized!"));
	 	return;
	 }
	 InputComponent->BindKey(EKeys::W, IE_Pressed,this,&AAirshipController::IncreaseThrottleZ);
	// InputComponent->BindKey(EKeys::S,IE_Pressed,this,&AAirshipController::DecreaseThrottleZ);
	// InputComponent->BindKey(EKeys::D,IE_Pressed,this,&AAirshipController::IncreaseThrottleX);
	// InputComponent->BindKey(EKeys::A,IE_Pressed,this,&AAirshipController::DecreaseThrottleX);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(IncreaseThrustZ, ETriggerEvent::Triggered,this,&AAirshipController::IncreaseThrottleZ);
	}
	
}

void AAirshipController::IncreaseThrottleZ()
{
	if (ControlledAirship) ControlledAirship->AddThrottle(0.1f);
	else UE_LOG(LogTemp, Error, TEXT("NOTHING!"));
}

void AAirshipController::DecreaseThrottleZ()
{
	if (ControlledAirship) ControlledAirship->AddThrottle(-0.1f);
}

void AAirshipController::IncreaseThrottleX()
{
	if (ControlledAirship) ControlledAirship->AddThrottle(0.1f);
}

void AAirshipController::DecreaseThrottleX()
{
	if (ControlledAirship) ControlledAirship->AddThrottle(-0.1f);
}


void AAirshipController::UpdateThrottle(float Value)
{
}

void AAirshipController::SetControlledAirship()
{
	
	if (APawn* CurrentPawn = GetPawn())
	{
		ControlledAirship = Cast<AAirship>(CurrentPawn);
        
		if (ControlledAirship)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully set Controlled Airship: %s"), *ControlledAirship->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pawn is not an Airship! Pawn Type: %s"), *CurrentPawn->GetClass()->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn assigned to this controller"));
	}
}
