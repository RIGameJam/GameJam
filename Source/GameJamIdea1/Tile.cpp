// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ActorPool.h"
#include "NavMesh/NavMeshBoundsVolume.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBounds = FBox(FVector(0, -2000, 0), FVector(4000, 2000, 0));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	Super::EndPlay(EndPlayReason);
	if (Volume)
		NMBVPool -> Return(Volume);
}


// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


template<class T>
TArray<AActor*> ATile::RandomlyPlaceActors(TSubclassOf<T> SpawnClass, int32 NumberMin, int32 NumberMax, float Radius, float MinScale, float MaxScale) 
{
	int32 Number = FMath::RandRange(NumberMin, NumberMax);
	TArray<AActor*> ArrayOfActors;
	for (int32 i = 0; i < Number; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		Radius *= SpawnPosition.Scale;
		if (GetEmptyLocation(SpawnPosition.Location, Radius, SpawningBounds)) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			AActor* Actor = PlaceActor(SpawnClass, SpawnPosition);
			if (Actor) {
				ArrayOfActors.AddUnique(Actor);
			}
		}
	}
	return ArrayOfActors;
}


TArray<AActor*> ATile::PlaceActors(TSubclassOf<AActor> SpawnClass, int32 NumberMin, int32 NumberMax, float Radius, float MinScale, float MaxScale) 
{
	return RandomlyPlaceActors(SpawnClass, NumberMin, NumberMax, Radius, MinScale, MaxScale);
}

TArray<AActor*> ATile::PlaceAIPawns(TSubclassOf<APawn> SpawnClass, int32 NumberMin, int32 NumberMax, float Radius) 
{
	return RandomlyPlaceActors(SpawnClass, NumberMin, NumberMax, Radius, 1, 1);
}


TArray<FSpawnPosition> ATile::GenerateSpawnPositions(int32 NumberMin, int32 NumberMax, float Radius, float MinScale, float MaxScale) 
{
	TArray<FSpawnPosition> SpawnPositions;
	int32 Number = FMath::RandRange(NumberMin, NumberMax);
	for (int32 i = 0; i < Number; i++) {
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		Radius *= SpawnPosition.Scale;
		if (GetEmptyLocation(SpawnPosition.Location, Radius, SpawningBounds)) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}
	return SpawnPositions;
}




bool ATile::SphereCast(FVector StartLocation, float Radius) 
{
	FHitResult OutHitResult;
	return GetWorld() -> SweepSingleByChannel(OutHitResult, StartLocation, StartLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
}

bool ATile::GetEmptyLocation(FVector& OutPos, float Radius, FBox Bounds) 
{
	int32 MaxAttemptsToSpawn = 10;
	for (int32 i = 0; i < MaxAttemptsToSpawn; i++)	 {
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(SpawnPoint, Radius)) {
			OutPos = SpawnPoint;
			return true;
		}
	}

	return false;
}

AActor* ATile::PlaceActor(TSubclassOf<AActor> ActorClassToSpawn, const FSpawnPosition& SpawnPosition) 
{
	FActorSpawnParameters SpawnInfo; 
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Forces the pawn to spawn even if colliding
	AActor* SpawnedActor = GetWorld() -> SpawnActor<AActor>(ActorClassToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0));
	SpawnedActor -> SetActorScale3D(FVector(SpawnPosition.Scale));
	SpawnedActor -> AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	return SpawnedActor;
}

AActor* ATile::PlaceActor(TSubclassOf<APawn> ActorClassToSpawn, const FSpawnPosition& SpawnPosition) 
{
	FActorSpawnParameters SpawnInfo; 
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Forces the pawn to spawn even if colliding
	APawn* SpawnedActor = GetWorld() -> SpawnActor<APawn>(ActorClassToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0), SpawnInfo);
	SpawnedActor -> AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedActor -> SpawnDefaultController();
	return SpawnedActor;
}

bool ATile::CanSpawnAtLocation(FVector StartLocation, float Radius) 
{
	StartLocation = ActorToWorld().TransformPosition(StartLocation);
	bool HasHit = SphereCast(StartLocation, Radius);
	return !HasHit;
}


void ATile::SetNMBVPool(UActorPool* Pool) 
{
	NMBVPool = Pool;
	PositionNavMeshBoundsVolume();
}


void ATile::PositionNavMeshBoundsVolume() 
{
	Volume = NMBVPool -> Checkout();
	if (!ensure(Volume)) return;
	Volume -> SetActorLocation(GetActorLocation() + NavMeshSpawningOffset);
	FNavigationSystem::Build(*GetWorld());
}
