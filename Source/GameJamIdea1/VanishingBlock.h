// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.h"
#include "Vanishable.h"
#include "VanishingBlock.generated.h"

UCLASS()
class GAMEJAMIDEA1_API AVanishingBlock : public AActor, public ITriggerable, public IVanishable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVanishingBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GetIsRewinding(bool& bIsRewinding);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual TArray<AActor*>& GetUniqueTriggers() override;
	virtual int32& GetNumberOfTriggersRequired() override;

	virtual void MarkDirty() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeVanishStatus(bool bVisible);

	UFUNCTION()
	void AfterDelay(); // I have to do this because of FLatentActionInfo

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	bool bTriggered = false;

	TArray<AActor*> UniqueTriggers;

	UPROPERTY(EditAnywhere)
	int32 NumberOfTriggersReq;

private:
	FTimerHandle TimerHandle;

};
