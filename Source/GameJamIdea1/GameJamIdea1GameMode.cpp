// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJamIdea1GameMode.h"
#include "GameJamIdea1HUD.h"
#include "GameJamIdea1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AGameJamIdea1GameMode::AGameJamIdea1GameMode()
		: Super()
{
	// use our custom HUD class
	HUDClass = AGameJamIdea1HUD::StaticClass();
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameJamIdea1GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInterping)
	{
		float amount = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(this), TargetGlobalTimeDilation, DeltaTime, 10.f);
		UGameplayStatics::SetGlobalTimeDilation(this, amount);
		if (FMath::IsNearlyEqual(amount, TargetGlobalTimeDilation, .0001f))
		{
			bIsInterping = false;
			UGameplayStatics::SetGlobalTimeDilation(this, TargetGlobalTimeDilation);
		}
	}
}

void AGameJamIdea1GameMode::SetNewTargetGTD(float Value)
{
	TargetGlobalTimeDilation = Value;
	bIsInterping = true;
}