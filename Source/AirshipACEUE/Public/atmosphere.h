#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "atmosphere.generated.h"

UCLASS(Blueprintable)
class AIRSHIPACEUE_API UAtmosphere : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UAtmosphere();
	//function to update values
	void UpdateAtmosphere(float Altitude);
	//get/sets for those private values
	float GetTemperature() const { return Temperature; }
	float GetPressure() const { return Pressure; }
	float GetDensity() const { return Density; }

	// Initialisation
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// Deinitialize override

	virtual void Deinitialize() override;

	// Atmosphere calculations
	UFUNCTION(BlueprintCallable, Category="Atmosphere")
	float CalculateTemperature(float Altitude) const;

	UFUNCTION(BlueprintCallable, Category="Atmosphere")
	float CalculatePressure(float Altitude) const;

	UFUNCTION(BlueprintCallable, Category="Atmosphere")
	float CalculateDensity() const;
	
private:
	float Temperature;
	float Pressure;
	float Density;
};
