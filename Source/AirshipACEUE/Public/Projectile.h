// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class AIRSHIPACEUE_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;
	
	// Initial velocity of the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	FVector InitialVelocity;

	// Launch angle (in degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	float LaunchAngle;

	// Mass of the projectile (kg)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	float Mass;

	// Current position of the projectile (world-space, derived from Unreal's transform system)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	FVector CurrentPosition;

	// Movement component to handle projectile motion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	int ProjectileType;

	// Function to initialize the projectile's motion
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void InitialiseProjectile(float Velocity, float Angle);

	// Event triggered when the projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
