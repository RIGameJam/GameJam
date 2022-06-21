// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.h"
#include "Door.generated.h"

UCLASS()
class GAMEJAMIDEA1_API ADoor : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	bool bOpen = false;

	UPROPERTY(EditAnywhere)
	float DoorSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	int32 NumberOfTriggersRequired = 1.f;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BR;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FL;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BL;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FR;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DoorPlate;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MarkDirty() override;

	virtual TArray<AActor*>& GetUniqueTriggers() override;
	virtual int32& GetNumberOfTriggersRequired() override;

private:
	TArray<AActor*> UniqueTriggers;

};
