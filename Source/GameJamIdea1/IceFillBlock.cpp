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

void AIceFillBlock::Freeze() 
{
	bFreeze = true;
}

// Called when the game starts or when spawned
void AIceFillBlock::BeginPlay()
{
	Super::BeginPlay();

	DynMat = Mesh -> CreateAndSetMaterialInstanceDynamicFromMaterial(0, Mat);

	CollisionComp -> OnComponentBeginOverlap.AddDynamic(this, &AIceFillBlock::OnBoxBeginOverlap);
	CollisionComp -> OnComponentEndOverlap.AddDynamic(this, &AIceFillBlock::OnBoxEndOverlap);
	
	bFreeze = false;

	CheckForActorsAlreadyOnPlate();
	ChangeStatus((uint8)EIFBStatus::NEUTRAL);
	UE_LOG(LogTemp, Warning, TEXT("Neutral is %i"), (uint8)EIFBStatus::NEUTRAL);

}


// Called every frame
void AIceFillBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AIceFillBlock::CheckForActorsAlreadyOnPlate() 
{
	if (ensure(CharBaseClass)) {
		TArray<AActor*> OverlappingActors;
		CollisionComp -> GetOverlappingActors(OverlappingActors, CharBaseClass);
		for (AActor* OverlappingActor : OverlappingActors) {
			if (!ActorsOnPlate.Contains(OverlappingActor)) {
				ActorsOnPlate.AddUnique(OverlappingActor);
			}
		}
		MarkDirty();
	}
}

void AIceFillBlock::OnTrigger() 
{
	bool bIsRewinding;
	GetIsRewinding(bIsRewinding);
	UE_LOG(LogTemp, Warning, TEXT("Rewinding, %i"), bIsRewinding)
	if (bIsRewinding) {
		DecrementStatus();
	}
	else {
		IncrementStatus();
	}
}

void AIceFillBlock::OnUnTrigger() 
{
}

void AIceFillBlock::ChangeStatus(uint8 FinalStatusInt) 
{
	EIFBStatus FinalStatus = (EIFBStatus)FinalStatusInt;
	switch(FinalStatus) {
		case EIFBStatus::NEUTRAL:
			SetDynColour(NeutralColour);
			break;
		case EIFBStatus::GOOD:
			SetDynColour(GoodColour);
			break;
		case EIFBStatus::BAD:
			SetDynColour(BadColour);
			break;
	}
	Status = FinalStatus;
	OnStatusChanged(FinalStatus);
}

void AIceFillBlock::IncrementStatus() 
{
	if (Status != EIFBStatus::BAD) {
		ChangeStatus((uint8)Status + 1);
	}
}

void AIceFillBlock::DecrementStatus() 
{
	if (Status != EIFBStatus::NEUTRAL) {
		ChangeStatus((uint8)Status - 1);
	}
}

void AIceFillBlock::SetDynColour(FLinearColor Colour) 
{
	if (!ensure(DynMat)) return;
	DynMat -> SetVectorParameterValue(TEXT("Colour"), Colour);
}

void AIceFillBlock::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (bFreeze) return;
	if (!ActorsOnPlate.Contains(Other)) {
		ActorsOnPlate.AddUnique(Other);
		bool bIsRewinding;
		GetIsRewinding(bIsRewinding);
		if (!bIsRewinding) {
			IncrementStatus();
		}
		UE_LOG(LogTemp, Warning, TEXT("It is %i"), (uint8)Status);
	}
}

void AIceFillBlock::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (bFreeze) return;
	if (ActorsOnPlate.Contains(Other)) {
		ActorsOnPlate.Remove(Other);
		bool bIsRewinding;
		GetIsRewinding(bIsRewinding);
		if (bIsRewinding) {
			DecrementStatus();
		}
	}
}

void AIceFillBlock::MarkDirty() 
{
}
