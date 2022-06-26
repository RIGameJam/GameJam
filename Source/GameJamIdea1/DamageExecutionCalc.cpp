// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionCalc.h"
#include "AttributeSetBase.h"
#include "ProjectCharacter.h"

struct DamageStatics {
  DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
  DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage);

  DamageStatics() {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackDamage, Target, true);
  }
};

static const DamageStatics& GetDamageStatics() {
  static DamageStatics Statics;
  return Statics;
}


UDamageExecutionCalc::UDamageExecutionCalc() 
{
  HeadshotMultiplier = 3.5f;
  RelevantAttributesToCapture.Add(GetDamageStatics().HealthDef);
  RelevantAttributesToCapture.Add(GetDamageStatics().AttackDamageDef);
}

void UDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const 
{
  UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
  if (!TargetASC) return;
  AActor* TargetActor = TargetASC -> GetAvatarActor();
  if (!TargetActor) return;

  FName HeadBoneName = "head";
  
  AProjectCharacter* TargetPC = Cast<AProjectCharacter>(TargetActor);
  if (TargetPC) {
    if (TargetPC -> HeadBoneName != "") {
      HeadBoneName = TargetPC -> HeadBoneName;
    }
  }

  const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

  const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
  
  FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float SourceAttackDamage;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AttackDamageDef, EvaluationParameters, SourceAttackDamage);  

  float FinalDamage = SourceAttackDamage;

  const FHitResult* Hit = Spec.GetContext().GetHitResult();
  // if (!Hit) {
  //   UE_LOG(LogTemp, Warning, TEXT("Hit doesn't exist"));
  // }
  // if (Spec.GetContext().IsValid()) {
  //   UE_LOG(LogTemp, Warning, TEXT("ctx is valid"));
  // }
  //   UE_LOG(LogTemp, Warning, TEXT("string is: %s"), *Spec.GetContext().ToString());
  // if (AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("effect.damage.canheadshot")))) {
  //   UE_LOG(LogTemp, Warning, TEXT("can headshot"));
  // }
  AActor* OrigInst = Spec.GetContext().GetOriginalInstigator();
  AActor* Inst = Spec.GetContext().GetInstigator();
  if (OrigInst) UE_LOG(LogTemp, Warning, TEXT("Orig inst: %s"), *OrigInst -> GetName());
  if (Inst) UE_LOG(LogTemp, Warning, TEXT("Inst: %s"), *Inst -> GetName());

  UE_LOG(LogTemp, Warning, TEXT("None"));

  // Hit && Hit->BoneName == HeadBoneName
  if (AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("effect.damage.canheadshot"))) && AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("effect.damage.headshot")))) {
    FinalDamage *= HeadshotMultiplier;
  }
  OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
} 

