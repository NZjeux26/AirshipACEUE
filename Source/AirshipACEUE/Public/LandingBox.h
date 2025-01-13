// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LandingBox.generated.h"

UCLASS()
class AIRSHIPACEUE_API ALandingBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALandingBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Overlap event function
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	// Optional: Add a custom property or action
	UPROPERTY(EditAnywhere, Category = "EndZone")
	TSubclassOf<class UUserWidget> MenuWidgetClass;
};

