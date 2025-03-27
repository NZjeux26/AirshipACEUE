#pragma once

#include "CoreMinimal.h"
#include "Engines.h"
#include "InputMappingContext.h"
#include "Weapon.h"
#include "WeaponHardpoint.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Airship.generated.h"

UCLASS()
class AIRSHIPACEUE_API AAirship : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAirship();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Static mesh component representing the airship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship")
	UStaticMeshComponent* AirshipMesh;
	// Airship physical properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float Diameter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	float Volume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	float FrontalArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	float LateralArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float CD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float DryMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float FuelMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float BallastMass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float CargoMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float WeaponMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	float MunitionsMass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	float TotalMass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	float EngineMass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	int NumEngines;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	FVector Position;
	
	// Attribute to define how many hardpoints the airship can have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	int NumHardpoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* AirshipCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engines")
	TArray<UEngines*> Engines; // Holds references to all engines
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Engines")
	FVector Throttle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	TArray<UWeaponHardpoint*> WeaponHardpoints;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void AimWeaponsAtMouse(float DeltaTime, UWeaponHardpoint* Hardpoint);

	// Function to spawn and attach weapons to the hardpoints
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EquipWeapons();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* AirshipMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveZ;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveNz;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveB;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveX;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ZeroPower;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FireWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ReloadWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair")
	FVector CrosshairWorldPosition;
	
	float MaxThrottle;
	float MinThrottle;
	FVector PreviousPosition;
	FVector SmoothedVelocity;
	FVector SmoothedAcceleration; 
	FVector SmoothedNetForce;
	FVector SmoothedThrust;
	FVector SmoothedPower; 
	FVector SmoothedBForce;
	FVector SmoothedDragForce;
	// Recalculate dimensions based on the mesh
	void UpdateDimensionsFromMesh();
	void SetDefaultPawn();
	void EquipEngines();
	
	FVector CalDrag(float Density) const;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateCrosshairPosition();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupCrossHairWidget();
	
	void MoveZAxis();
	void MoveNzAxis();
	void MoveBAxis();
	void MoveXAxis();
	void ZeroPowerAxis();
	void FireWeapons();
	void ReloadWeapons();

public:
	// Function to set scale and recalculate dimensions
	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale(float ScaleFactor);

	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale3D(FVector ScaleFactor);
	
	// Getter for FuelMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetFuelMass() const { return FuelMass; }

	// Setter for FuelMass *Might need to add checking for just numbers and not characters
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	void SetFuelMass(float NewFuelMass);

	// Getter for CargoMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetCargoMass() const { return CargoMass; }

	// Setter for CargoMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	void SetCargoMass(float NewCargoMass); 

	// Getter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetBallastMass() const { return BallastMass; }

	// Setter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	void SetBallastMass(float NewBallastMass);

	// Getter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetWeaponsMass() const { return WeaponMass; }
	
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
    void GetEngineMass(UEngines* Engine);
	
	// Setter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	void SetWeaponsMass(float NewWeaponsMass); 

	// Getter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetTotalMass() const { return TotalMass; }

	// Getter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	float GetDryMass() const { return DryMass; }

	// Setter for BallastMass
	UFUNCTION(BlueprintCallable, Category = "Airship Properties")
	void UpdateTotalMass();

	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	FVector GetCrosshairWorldPosition();
	
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	TArray<UWeaponHardpoint*> GetWeaponHardpoints() const;

};
