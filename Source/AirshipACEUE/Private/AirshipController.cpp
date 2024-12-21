/*
// Fill out your copyright notice in the Description page of Project Settings.

#include "AirshipController.h"
#include "Airship.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"

// Called when the game starts or the controller is initialized
void AAirshipController::BeginPlay()
{
    Super::BeginPlay();

    // Add the input mapping context to the player's Enhanced Input Subsystem
    if (APlayerController* PC = Cast<APlayerController>(this))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("InputMappingContext is null in AirshipController!"));
        }
    }
    SetupInputComponent();
    SetControlledAirship();
}

// Called to bind functionality to input
void AAirshipController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UE_LOG(LogTemp, Log, TEXT("SetupInputComponent called"));
    // Ensure we have an Enhanced Input Component
    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (IncreaseThrustZ)
        {
            EnhancedInputComp->BindAction(IncreaseThrustZ, ETriggerEvent::Triggered, this, &AAirshipController::IncreaseThrottleZ);
        }

        if (DecreaseThrustZ)
        {
            EnhancedInputComp->BindAction(DecreaseThrustZ, ETriggerEvent::Triggered, this, &AAirshipController::DecreaseThrottleZ);
        }

        if (IncreaseThrustX)
        {
            EnhancedInputComp->BindAction(IncreaseThrustX, ETriggerEvent::Triggered, this, &AAirshipController::IncreaseThrottleX);
        }

        if (DecreaseThrustX)
        {
            EnhancedInputComp->BindAction(DecreaseThrustX, ETriggerEvent::Triggered, this, &AAirshipController::DecreaseThrottleX);
        }#1#
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent not found on InputComponent!"));
    }
}

// Increases throttle along the Z-axis
void AAirshipController::IncreaseThrottleZ()
{
    if (ControlledAirship)
    {
        //ControlledAirship->AddThrottle(0.1f);
        UE_LOG(LogTemp, Log, TEXT("Increased throttle Z-axis by 0.1"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controlled Airship not set!"));
    }
}

// Decreases throttle along the Z-axis
void AAirshipController::DecreaseThrottleZ()
{
    if (ControlledAirship)
    {
       // ControlledAirship->AddThrottle(-0.1f);
        UE_LOG(LogTemp, Log, TEXT("Decreased throttle Z-axis by 0.1"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controlled Airship not set!"));
    }
}

// Increases throttle along the X-axis
void AAirshipController::IncreaseThrottleX()
{
    if (ControlledAirship)
    {
        //ControlledAirship->AddThrottle(0.1f);
        UE_LOG(LogTemp, Log, TEXT("Increased throttle X-axis by 0.1"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controlled Airship not set!"));
    }
}

// Decreases throttle along the X-axis
void AAirshipController::DecreaseThrottleX()
{
    if (ControlledAirship)
    {
       // ControlledAirship->AddThrottle(-0.1f);
        UE_LOG(LogTemp, Log, TEXT("Decreased throttle X-axis by 0.1"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controlled Airship not set!"));
    }
}

void AAirshipController::UpdateThrottle(float Value)
{
   // AAirship::Throttle
}

// Sets the controlled airship to the currently possessed pawn
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
*/
