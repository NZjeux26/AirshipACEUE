#pragma once

#include "CoreMinimal.h"
#include "Engines.h"
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
	float Mass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	int NumEngines;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airship Properties")
	FVector Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* AirshipCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engines")
	TArray<UEngines*> Engines; // Holds references to all engines
	
	// Recalculate dimensions based on the mesh
	void UpdateDimensionsFromMesh();

	void SetDefaultPawn();

	FVector CalDrag(float Density) const;
	
public:

	// Function to set scale and recalculate dimensions
	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale(float ScaleFactor);

	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale3D(FVector ScaleFactor);
	
};
