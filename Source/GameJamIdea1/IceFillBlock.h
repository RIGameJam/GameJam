// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceFillBlock.generated.h"


// Debated on whether to make this a subclass of APressurePlate but decided to make it a new class instead


UENUM(BlueprintType) 
enum class EIFBStatus : uint8 {
	NEUTRAL UMETA(DisplayName = "NEUTRAL"),
	GOOD UMETA(DisplayName = "GOOD"),
	BAD UMETA(DisplayName = "BAD")
};


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


	int32 ActorsTouched = 0;

	UPROPERTY(BlueprintReadOnly)
	EIFBStatus Status = EIFBStatus::NEUTRAL;
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetIsRewinding(bool& bIsRewinding);

	UFUNCTION(BlueprintCallable)
	void Freeze();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor NeutralColour;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor GoodColour;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BadColour;
	
	void CheckForActorsAlreadyOnPlate();

	void OnTrigger();
	void OnUnTrigger();

	UFUNCTION(BlueprintCallable)
	void ChangeStatus(uint8 FinalStatusInt);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStatusChanged(EIFBStatus NewStatus);
	
	void IncrementStatus();
	void DecrementStatus();

	void SetDynColour(FLinearColor Colour);

public:	

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MarkDirty();

	bool bFreeze = false;

};
