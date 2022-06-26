// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "AbilityTypes.h"
#include "TaggedActor.h"
#include "Containers/CircularBuffer.h"

#include "ProjectCharacter.generated.h"

USTRUCT(BlueprintType) 
struct FRewindInformation {
	GENERATED_BODY()

	FRewindInformation() {
		TransformArr = TArray<FTransform>();
		VectorArr = TArray<FVector>();
	}
	FRewindInformation(TArray<FTransform> TArr, TArray<FVector> VArr) {
		TransformArr = TArr;
		VectorArr = VArr;
	}

	UPROPERTY(BlueprintReadWrite)
	TArray<FTransform> TransformArr;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> VectorArr;
};

UCLASS()
class GAMEJAMIDEA1_API AProjectCharacter : public ACharacter, public IAbilitySystemInterface, public ITaggedActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual class UAbilitySystemComponent *GetAbilitySystemComponent() const { return SystemComp; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	class AGameJamIdea1GameMode *GameMode;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;
	
	UFUNCTION(BlueprintCallable)
	bool IsOtherActorHostile(AActor* OtherActor);

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadWrite)
	AActor* LastDamagedBy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDamageIndicatorComponent> DICClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> TransformBufferArr;
	UPROPERTY(EditDefaultsOnly)
	int32 TBACapacity = 7;
	UPROPERTY(BlueprintReadOnly)
	int32 CurTBAIndex = -1;

	UPROPERTY(BlueprintReadWrite)
	int32 CurTimeIndex = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxTime = 200;

	UFUNCTION(BlueprintCallable)
	void ResetTime();

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> VelocityBufferArr;
	UPROPERTY(EditDefaultsOnly)
	int32 VBACapacity = 7;
	UPROPERTY(BlueprintReadOnly)
	int32 CurVBAIndex = -1;

		UPROPERTY(BlueprintReadOnly)
	TArray<float> FloatBufferArr;
	int32 CurFBAIndex = 0;
	int32 FBACapacity = 7;
	int32 CurNumber = 0;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerHandle;

	template<class T>
	T Retrieve(const TArray<T>& ArrayToModify, int32& index);

	UFUNCTION(BlueprintCallable)
	FTransform RetrieveTransform(); // This is here because blueprints cannot call templated functions
	UFUNCTION(BlueprintCallable)
	FVector RetrieveVelocity(); // This is here because blueprints cannot call templated functions

	UFUNCTION(BlueprintCallable)
	void AdvanceTime();
	UFUNCTION(BlueprintCallable)
	void ReverseTime();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckTime();

	UFUNCTION(BlueprintCallable)
	bool SaveCurValues();


	TArray<TArray<FTransform>> TransformsArr;
	TArray<TArray<FVector>> VelocitiesArr;
	
	// TMap<TArray<FTransform>, TArray<FVector>> TVMap;
	UPROPERTY(BlueprintReadOnly)
	TArray<FRewindInformation> InfoArr;

	UFUNCTION(BlueprintCallable)
	void PauseRecording();
	UFUNCTION(BlueprintCallable)
	void ResetRecordingAndTime();


	UPROPERTY(BlueprintReadWrite)
	bool bIsRewinding = false;

	UPROPERTY(BlueprintReadOnly)
	int32 Tindex = -1;

	UPROPERTY(EditDefaultsOnly)
	FName HeadBoneName;


	// FTransform Retrieve(const TArray<FTransform>& ArrayToModify);
	// FVector Retrieve(const TArray<FVector>& ArrayToModify);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool bRecordSelf = false;

	UPROPERTY(EditDefaultsOnly)
	bool bShowDamageIndicatorOnHit = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAbilitySystemComponent *SystemComp = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectCharacter")
	class UAttributeSetBase *AttributeSetBase;

	UFUNCTION(BlueprintCallable)
	void AcquireAbility(TSubclassOf<class UGameplayAbility> AbilityToAcquire);
	UFUNCTION(BlueprintCallable)
	void AcquireAbilities(TArray<TSubclassOf<class UGameplayAbility>> AbilitiesToAcquire);

	UFUNCTION(BlueprintCallable)
	void AddGameplayTag(const struct FGameplayTag& Tag);
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag(const FGameplayTag& Tag);
	UFUNCTION(BlueprintCallable)
	void CancelAbilities(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, class UGameplayAbility* Ignore=nullptr);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth, bool FullHealth, float PreviousHealth, AActor *EffectInstigator);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnMainAttributeChanged"))
	void BP_OnMainAttributeChanged(EAttributeType Type, float CurrentAttributeValue);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeath"))
	void BP_OnDeath();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth, bool FullHealth, float PreviousHealth, AActor *EffectInstigator);
	UFUNCTION()
	void OnManaChanged(float Mana, float MaxMana);
	UFUNCTION()
	void OnMainAttributeChanged(EAttributeType Type, float CurrentAttributeValue);
	void SetTeamIDByControllerType();

	UFUNCTION()
	void OnRecord();


	void ShiftDown(FTransform TransformToSet, FVector VectorToSet);
	void ShiftDownTransform(FTransform TransformToSet);
	void ShiftDownVelocity(FVector VectorToSet);
	void ShiftDownFloat(float ValueToSet);
	
	template<class T>
	void ShiftDown(const TArray<T>& ArrayToModify, T ValueToSet, int32& index);
};
