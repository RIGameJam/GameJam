// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Triggerable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UTriggerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEJAMIDEA1_API ITriggerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual bool Trigger(AActor* Trigger);
	UFUNCTION(BlueprintCallable)
	virtual bool UnTrigger(AActor* Trigger);

	UFUNCTION(BlueprintCallable)
	virtual void MarkDirty() = 0;

	// UFUNCTION(BlueprintCallable)
	virtual TArray<AActor*>& GetUniqueTriggers() = 0;
	virtual int32& GetNumberOfTriggersRequired() = 0;

};
