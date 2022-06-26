// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UActorPool::Add(AActor* Volume) 
{
	if (ensure(Volume)) {
		Pool.Push(Volume);
	}
}


void UActorPool::Return(AActor* Volume) 
{
	Add(Volume);
}


AActor* UActorPool::Checkout() 
{
	if (Pool.Num() > 0) {
		return Pool.Pop();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Not enough actors in pool"));
	}
	return nullptr;
}