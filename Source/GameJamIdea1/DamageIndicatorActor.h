// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageIndicatorActor.generated.h"

UCLASS()
class GAMEJAMIDEA1_API ADamageIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageIndicatorActor();

	UFUNCTION(BlueprintImplementableEvent)
	void Initialise(const FText& InText);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector InitialLocation;
};
