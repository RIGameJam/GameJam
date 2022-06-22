// Fill out your copyright notice in the Description page of Project Settings.


#include "VanishingBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AVanishingBlock::AVanishingBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root -> SetRelativeLocation(FVector(0));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh -> SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AVanishingBlock::BeginPlay()
{
	Super::BeginPlay();

	if (GetNumberOfTriggersRequired() > 0) ChangeVanishStatus(false);
	
}

// Called every frame
void AVanishingBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AActor*>& AVanishingBlock::GetUniqueTriggers() 
{
	return UniqueTriggers;
}

int32& AVanishingBlock::GetNumberOfTriggersRequired() 
{
	return NumberOfTriggersReq;
}

void AVanishingBlock::ChangeVanishStatus_Implementation(bool bVisible) 
{
	Mesh -> SetVisibility(bVisible, true);
	Mesh -> SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void AVanishingBlock::AfterDelay() 
{
	if (!bTriggered) {
		ChangeVanishStatus(false);
		bTriggered = false;
	}
}

void AVanishingBlock::MarkDirty() 
{
	if (GetUniqueTriggers().Num() >= GetNumberOfTriggersRequired()) {
		ChangeVanishStatus(true);
		bTriggered = true;
	}
	else {
		bTriggered = false;
		bool bIsRewinding;
		GetIsRewinding(bIsRewinding);
		if (bIsRewinding) {
			AfterDelay();
		}
		else {
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AVanishingBlock::AfterDelay, .5f, false);
		}
	}
}

