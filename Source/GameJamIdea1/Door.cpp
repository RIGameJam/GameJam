// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root -> SetRelativeLocation(FVector(0));
	SetRootComponent(Root);

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame -> SetupAttachment(Root);

	BR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BR"));
	BR -> SetupAttachment(DoorFrame);
	FL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FL"));
	FL -> SetupAttachment(DoorFrame);
	BL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BL"));
	BL -> SetupAttachment(DoorFrame);
	FR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FR"));
	FR -> SetupAttachment(DoorFrame);

	DoorPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorPlate"));
	DoorPlate -> SetupAttachment(DoorFrame);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpen) {
		BR -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(BR -> GetRelativeLocation().Y, 110.f, DeltaTime, DoorSpeed), 0.f));
		FL -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(FL -> GetRelativeLocation().Y, -110.f, DeltaTime, DoorSpeed), 0.f));
		BL -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(BL -> GetRelativeLocation().Y, -110.f, DeltaTime, DoorSpeed), 0.f));
		FR -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(FR -> GetRelativeLocation().Y, 110.f, DeltaTime, DoorSpeed), 0.f));
	}
	else {
		BR -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(BR -> GetRelativeLocation().Y, 0.f, DeltaTime, DoorSpeed), 0.f));
		FL -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(FL -> GetRelativeLocation().Y, 0.f, DeltaTime, DoorSpeed), 0.f));
		BL -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(BL -> GetRelativeLocation().Y, 0.f, DeltaTime, DoorSpeed), 0.f));
		FR -> SetRelativeLocation(FVector(0.f, FMath::FInterpTo(FR -> GetRelativeLocation().Y, 0.f, DeltaTime, DoorSpeed), 0.f));
	}

}

void ADoor::MarkDirty() 
{
	bOpen = GetUniqueTriggers().Num() >= GetNumberOfTriggersRequired();
}

TArray<AActor*>& ADoor::GetUniqueTriggers() 
{
	return UniqueTriggers;
}

int32& ADoor::GetNumberOfTriggersRequired() 
{
	return NumberOfTriggersRequired;
}
