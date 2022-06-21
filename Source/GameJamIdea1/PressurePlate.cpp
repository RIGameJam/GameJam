// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectCharacter.h"
#include "TimerManager.h"

// Sets default values
APressurePlate::APressurePlate()
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
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	DynMat = Mesh -> CreateAndSetMaterialInstanceDynamicFromMaterial(0, Mat);

	CollisionComp -> OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnBoxBeginOverlap);
	CollisionComp -> OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnBoxEndOverlap);
	CheckForActorsAlreadyOnPlate();

	MarkDirty();

	if (bVanishing && bChangeScale) {
		Mesh -> SetRelativeScale3D(FVector(2.f, 2.f, Mesh -> GetRelativeScale3D().Z));
	}
	
}

void APressurePlate::MarkPlateDirty() 
{
	if (ActorsOnPlate.Num() > 0) {
		for (AActor* Actor : TriggerableActors) {
			ITriggerable* Trig = Cast<ITriggerable>(Actor);
			if (Trig) Trig -> Trigger(this);
		}
		OnTrigger();
	}
	else {
		for (AActor* Actor : TriggerableActors) {
			ITriggerable* Trig = Cast<ITriggerable>(Actor);
			if (Trig) Trig -> UnTrigger(this);
		}
		OnUnTrigger();
	}
}

void APressurePlate::ChangeVanishStatus_Implementation(bool bVisible) 
{
	Mesh -> SetVisibility(bVisible, true);
	Mesh -> SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	CollisionComp -> SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

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

void APressurePlate::OnTrigger() 
{
	if (!ensure(DynMat)) return;
		DynMat -> SetVectorParameterValue(TEXT("Colour"), TriggeredColour);
}

void APressurePlate::OnUnTrigger() 
{
	if (!ensure(DynMat)) return;
	DynMat -> SetVectorParameterValue(TEXT("Colour"), UnTriggeredColour);
}

void APressurePlate::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (!ActorsOnPlate.Contains(Other)) {
		ActorsOnPlate.AddUnique(Other);
		MarkPlateDirty();
	}
}

void APressurePlate::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (ActorsOnPlate.Contains(Other)) {
		ActorsOnPlate.Remove(Other);
		MarkPlateDirty();
	}
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AActor*>& APressurePlate::GetUniqueTriggers() 
{
	return UniqueTriggers;
}

int32& APressurePlate::GetNumberOfTriggersRequired() 
{
	return NumberOfTriggersReq;
}


void APressurePlate::MarkDirty() 
{
	if (bVanishing) {
		if (GetUniqueTriggers().Num() >= GetNumberOfTriggersRequired()) {
			ChangeVanishStatus(true);
			bTriggered = true;
		}
		else {
			bTriggered = false;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &APressurePlate::AfterDelay, .5f, false);
		}
	}
}

void APressurePlate::AfterDelay() 
{
	if (!bTriggered) {
		ChangeVanishStatus(false);
		bTriggered = false;
	}
}
