// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassComponent.h"



void UGrassComponent::BeginPlay() 
{
    Super::BeginPlay();
    SpawnGrass();
}



void UGrassComponent::SpawnGrass() 
{
    int32 Count = FMath::RandRange(MinCount, MaxCount);
	for (int i = 0; i < Count; i++) {
        FVector Location = FMath::RandPointInBox(SpawningExtents);
        AddInstance(FTransform(Location));
    }
}