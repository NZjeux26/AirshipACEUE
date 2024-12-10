#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Airship.generated.h"

UCLASS()
class AIRSHIPACEUE_API AAirship : public AActor
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airship Properties")
	FVector Velocity;

	
	// Recalculate dimensions based on the mesh
	void UpdateDimensionsFromMesh();

public:

	// Function to set scale and recalculate dimensions
	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale(float ScaleFactor);

	UFUNCTION(BlueprintCallable, Category = "Airship|Scaling")
	void SetAirshipScale3D(FVector ScaleFactor);
	
};
