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

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;
	
	UFUNCTION(BlueprintCallable)
	bool IsOtherActorHostile(AActor* OtherActor);

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadWrite)
	AActor* LastDamagedBy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDamageIndicatorComponent> DICClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> BufferArr;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool bRecordSelf = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAbilitySystemComponent *SystemComp = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectCharacter")
	class UAttributeSetBase *AttributeSetBase;

	UFUNCTION(BlueprintCallable)
	void AcquireAbility(TSubclassOf<class UGameplayAbility> AbilityToAcquire);
	UFUNCTION(BlueprintCallable)
	void AcquireAbilities(TArray<TSubclassOf<class UGameplayAbility>> AbilitiesToAcquire);

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

	void OnRecord();

	void ShiftDown(FTransform ValueToSet);
};
