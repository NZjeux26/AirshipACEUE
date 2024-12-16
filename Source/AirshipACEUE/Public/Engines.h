// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engines.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIRSHIPACEUE_API UEngines : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEngines();

	// Engine properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Properties")
	float Mass; // Mass of the engine in kilograms

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Properties")
	float FuelFlow; // Fuel consumption rate in kg/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Properties")
	float PropDiameter; // Propeller diameter in meters

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engine Properties")
	float PropArea; // Calculated propeller area in m^2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Properties")
	float PropEfficiency; // Propeller efficiency as a percentage (0.0 to 1.0)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Properties")
	float HorsePower; // Engine horsepower

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engine Properties")
	FVector Thrust; // Current thrust in Newtons

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float GetPropArea() const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void CalculateEngineThrust(float AirDensity, const FVector& AirVelocity);
};
