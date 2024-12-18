// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Airship.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "AirshipController.generated.h"

class AAirship;
UCLASS()
class AIRSHIPACEUE_API AAirshipController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual  void SetupInputComponent() override;
private:
	AAirship* ControlledAirship;
	void IncreaseThrottleZ();
	void DecreaseThrottleZ();
	void IncreaseThrottleX();
	void DecreaseThrottleX();
	void UpdateThrottle(float Value);

	void SetControlledAirship();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IncreaseThrustZ;
};
