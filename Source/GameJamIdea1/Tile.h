// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


USTRUCT()
struct FSpawnPosition {
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

UCLASS()
class GAMEJAMIDEA1_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> PlaceActors(TSubclassOf<AActor> SpawnClass, int32 NumberMin = 1, int32 NumberMax = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> PlaceAIPawns(TSubclassOf<APawn> SpawnClass, int32 NumberMin = 1, int32 NumberMax = 1, float Radius = 500);
	UFUNCTION(BlueprintCallable)
	void SetNMBVPool(class UActorPool* Pool);

private:
	bool SphereCast(FVector StartLocation, float Radius);
	bool GetEmptyLocation(FVector& OutPos, float Radius, FBox Bounds);
	AActor* PlaceActor(TSubclassOf<AActor> ActorClassToSpawn, const FSpawnPosition& SpawnPosition);
	AActor* PlaceActor(TSubclassOf<APawn> ActorClassToSpawn, const FSpawnPosition& SpawnPosition);
	bool CanSpawnAtLocation(FVector StartLocation, float Radius);
	UPROPERTY()
	class UActorPool* NMBVPool;
	void PositionNavMeshBoundsVolume();
	UPROPERTY()
	AActor* Volume = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FBox SpawningBounds = FBox(FVector(0, -2000, 0), FVector(4000, 2000, 0));
	UPROPERTY(EditDefaultsOnly)
	FVector NavMeshSpawningOffset = FVector(2000, 0, 0);
	TArray<FSpawnPosition> GenerateSpawnPositions(int32 NumberMin, int32 NumberMax, float Radius, float MinScale, float MaxScale);
	template<class T>
	TArray<AActor*> RandomlyPlaceActors(TSubclassOf<T> SpawnClass, int32 NumberMin = 1, int32 NumberMax = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);
	
};
