// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponHardpoint.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIRSHIPACEUE_API UWeaponHardpoint : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponHardpoint();

// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Called when the game starts
	virtual void BeginPlay() override;
	// The weapon blueprint to spawn on this hardpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoint")
	TSubclassOf<class AWeapon> WeaponToMount;

	// Optional: You can track the spawned weapon instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardpoint")
	class AWeapon* MountedWeapon;

	// Function to attach the weapon
	UFUNCTION(BlueprintCallable, Category = "Hardpoint")
	void AttachWeapon();
};
