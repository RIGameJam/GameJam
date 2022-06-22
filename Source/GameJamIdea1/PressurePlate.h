// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.h"
#include "Vanishable.h"
#include "PressurePlate.generated.h"

UCLASS()
class GAMEJAMIDEA1_API APressurePlate : public AActor, public ITriggerable, public IVanishable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlate();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> TriggerableActors; // I can't do TArray<ITriggerable*> because that's not allowed

	void MarkPlateDirty();

	UPROPERTY(EditDefaultsOnly)
	FLinearColor TriggeredColour;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnTriggeredColour;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeVanishStatus(bool bVisible);

	virtual TArray<AActor*>& GetUniqueTriggers() override;
	virtual int32& GetNumberOfTriggersRequired() override;

	virtual void MarkDirty() override;

	UPROPERTY(EditAnywhere)
	bool bVanishing = false;

	UPROPERTY(EditAnywhere)
	bool bChangeScale = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckForActorsAlreadyOnPlate();

	void OnTrigger();
	void OnUnTrigger();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTrigger();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUnTrigger();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	int32 NumberOfTriggersReq;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<AActor*> UniqueTriggers;
	bool bTriggered = false;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void AfterDelay();

};
