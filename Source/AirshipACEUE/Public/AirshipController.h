#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "AirshipController.generated.h"

// Forward declaration of AAirship
class AAirship;

UCLASS()
class AIRSHIPACEUE_API AAirshipController : public APlayerController
{
	GENERATED_BODY()

public:
	// Expose to blueprints for potential customization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> IncreaseThrustZ;
	// Input Mapping Context for Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
protected:
	// BeginPlay is called when the game starts or when the controller is spawned
	virtual void BeginPlay() override;

	// SetupInputComponent is called to bind input actions
	//virtual void SetupInputComponent() override;

private:
	// The airship controlled by this controller
	UPROPERTY()
	TObjectPtr<AAirship> ControlledAirship;

	

	// // Throttle control functions
	// void IncreaseThrottleZ();
	// void DecreaseThrottleZ();
	// void IncreaseThrottleX();
	// void DecreaseThrottleX();
	//
	// // Update throttle (optional for future use)
	// void UpdateThrottle(float Value);
	//
	// // Set the airship pawn controlled by this controller
	// void SetControlledAirship();
};
