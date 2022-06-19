// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAMIDEA1_API UDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecutionCalc();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	float HeadshotMultiplier = 3.5f;
	
};
