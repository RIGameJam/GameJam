// Fill out your copyright notice in the Description page of Project Settings.


#include "IceFillBlock.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectCharacter.h"

// Sets default values
AIceFillBlock::AIceFillBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComp -> SetRelativeLocation(FVector(0));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComp -> SetRelativeLocation(FVector(0));
	Mesh -> SetupAttachment(GetRootComponent());

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComp -> SetRelativeLocation(FVector(0));
	CollisionComp -> SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AIceFillBlock::BeginPlay()
{
	Super::BeginPlay();

	DynMat = Mesh -> CreateAndSetMaterialInstanceDynamicFromMaterial(0, Mat);

	CollisionComp -> OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnBoxBeginOverlap);
	CollisionComp -> OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnBoxEndOverlap);


	CheckForActorsAlreadyOnPlate();
	
}


// Called every frame
void AIceFillBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AIceFillBlock::CheckForActorsAlreadyOnPlate() 
{
	void APressurePlate::CheckForActorsAlreadyOnPlate() 
{
	if (ensure(CharBaseClass)) {
		TArray<AActor*> OverlappingActors;
		CollisionComp -> GetOverlappingActors(OverlappingActors, CharBaseClass);
		for (AActor* OverlappingActor : OverlappingActors) {
			if (!ActorsOnPlate.Contains(OverlappingActor)) {
				ActorsOnPlate.AddUnique(OverlappingActor);
			}
		}
		MarkPlateDirty();
	}
}
}
