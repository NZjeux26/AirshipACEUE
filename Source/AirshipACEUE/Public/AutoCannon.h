// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AutoCannon.generated.h"

UCLASS()
class AIRSHIPACEUE_API AAutoCannon : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAutoCannon();

	//Need a mesh for the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	// Dry mass of the weapon (kg)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	float ProjectileMass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	FVector ProjectileVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	int ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	float Angle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
