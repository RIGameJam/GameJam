// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEJAMIDEA1_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	int MinCount = 5;
	UPROPERTY(EditAnywhere)
	int MaxCount = 10;

	UPROPERTY(EditDefaultsOnly)
	FBox SpawningExtents = FBox(FVector(0, -2000, 0), FVector(4000, 2000, 0));
	
	void SpawnGrass();


};
