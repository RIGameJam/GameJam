// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceFillBlock.generated.h"


// Debated on whether to make this a subclass of APressurePlate but decided to make it a new class instead

UCLASS()
class GAMEJAMIDEA1_API AIceFillBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceFillBlock();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInstance* Mat;

	UPROPERTY(BlueprintReadWrite)
	class UMaterialInstanceDynamic* DynMat;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectCharacter> CharBaseClass;

	TArray<AActor*>	ActorsOnPlate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MarkPlateDirty();

	UPROPERTY(EditDefaultsOnly)
	FLinearColor TriggeredColour;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnTriggeredColour;
	
	void CheckForActorsAlreadyOnPlate();

	void OnTrigger();
	void OnUnTrigger();

public:	

};
