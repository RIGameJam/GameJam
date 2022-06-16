// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageIndicatorActor.h"

// Sets default values
ADamageIndicatorActor::ADamageIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
}

// Called every frame
void ADamageIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

