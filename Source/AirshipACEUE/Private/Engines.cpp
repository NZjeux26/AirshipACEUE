// Fill out your copyright notice in the Description page of Project Settings.


#include "Engines.h"


// Sets default values for this component's properties
UEngines::UEngines()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialise default values
	Mass = 0.0f;         // Default engine mass in kg
	FuelFlow = 0.0f;       // Default fuel flow in kg/s
	PropDiameter = 0.0f;   // Default propeller diameter in meters
	PropEfficiency = 0.0f; // Default propeller efficiency (80%)
	
	HorsePower = 0.0f;   // Default engine horsepower
	Thrust = 0.0f;
}


float UEngines::CalculateEngineThrust(float AirDensity, float AirVelocity)
{
	return 0;
}

// Called when the game starts
void UEngines::BeginPlay()
{
	Super::BeginPlay();
	PropArea = GetPropArea();
}

float UEngines::GetPropArea()
{
	float radius = PropDiameter / 2.0f;
	return PI * FMath::Pow(radius, 2);
}


// Called every frame
void UEngines::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

