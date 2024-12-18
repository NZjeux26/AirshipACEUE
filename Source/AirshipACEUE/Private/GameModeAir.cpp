// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeAir.h"
#include "Airship.h"
#include "AirshipController.h"

AGameModeAir::AGameModeAir()
{
	// Set the default pawn to your Airship class
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	PlayerControllerClass = AAirshipController::StaticClass();
}
