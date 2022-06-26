// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectCharacter.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.h"
#include "DamageIndicatorComponent.h"
#include "GameJamIdea1GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectCharacter::AProjectCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(FName("Ability System Component"));
  AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(FName("Attribute Set Base Component"));
}

// Called when the game starts or when spawned
void AProjectCharacter::BeginPlay() {
  Super::BeginPlay();

  AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(this);
  if (CurrentGameMode) {
    AGameJamIdea1GameMode* GM = Cast<AGameJamIdea1GameMode>(CurrentGameMode);
    if (GM) {
      GameMode = GM;
    }
  }

  if (ensure(AttributeSetBase)) {
    AttributeSetBase->OnHealthChanged.AddDynamic(this, &AProjectCharacter::OnHealthChanged);
    AttributeSetBase->OnManaChanged.AddDynamic(this, &AProjectCharacter::OnManaChanged);
    AttributeSetBase->OnMainAttributeChanged.AddDynamic(this, &AProjectCharacter::OnMainAttributeChanged);
  }
  
  bIsDead = false;

  if (bRecordSelf) {
    // BufferArr = TArray<float>({5.f, 3.f, 7.f});
    // UE_LOG(LogTemp, Warning, TEXT(Buffer.Capacity()));
    FloatBufferArr = TArray<float>();
    // FloatBufferArr.Init(-1.f, 7);
    // VelocityBufferArr = TArray<FVector(-99.f, -99.f, -99.f)
    ShiftDown(GetActorTransform(), GetVelocity());
    GetWorld() -> GetTimerManager().SetTimer(TimerHandle, this, &AProjectCharacter::OnRecord, .025f, true);
  }
}

// Called every frame
void AProjectCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AProjectCharacter::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire) {
  if (!ensure(SystemComp))
    return;
  if (!HasAuthority())
    return;
  if (AbilityToAcquire) {
    FGameplayAbilitySpecDef SpecDef = FGameplayAbilitySpecDef();
    SpecDef.Ability = AbilityToAcquire;
    FGameplayAbilitySpec Spec = FGameplayAbilitySpec(SpecDef, 1);
    SystemComp->GiveAbility(Spec);
  }
  SystemComp->InitAbilityActorInfo(this, this);
}

void AProjectCharacter::AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire) {
  if (HasAuthority()) {
    for (TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToAcquire) {
      AcquireAbility(AbilityClass);
    }
  }
}

void AProjectCharacter::AddGameplayTag(const FGameplayTag& Tag) 
{
	GetAbilitySystemComponent() -> AddLooseGameplayTag(Tag);
	GetAbilitySystemComponent() -> SetTagMapCount(Tag, 1);
}

void AProjectCharacter::RemoveGameplayTag(const FGameplayTag& Tag) 
{
	GetAbilitySystemComponent() -> RemoveLooseGameplayTag(Tag);
}

void AProjectCharacter::CancelAbilities(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, UGameplayAbility* Ignore) 
{
  GetAbilitySystemComponent() -> CancelAbilities(&WithTags, &WithoutTags, Ignore);
}

// Called to bind functionality to input
void AProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  PlayerInputComponent->BindAxis("MoveForward", this, &AProjectCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &AProjectCharacter::MoveRight);
}

bool AProjectCharacter::IsOtherActorHostile(AActor* OtherActor) { return IsOtherHostile(OtherActor); }

void AProjectCharacter::PossessedBy(AController* NewController) {
  Super::PossessedBy(NewController);
  SetTeamIDByControllerType();
}

void AProjectCharacter::ResetTime() 
{
  CurTimeIndex = 0;
  TransformBufferArr.Reset(MaxTime + 1);
  VelocityBufferArr.Reset(MaxTime + 1);
}

template<class T>
T AProjectCharacter::Retrieve(const TArray<T>& ArrayToModify, int32& index) 
{
  int32 Num = ArrayToModify.Num();
  if (index - 1 < 0) {
    index = Num - 1;
  }
  else if (index == Num) {
    index = Num - 1;
  }
  else {
    index --;
  }
  T Value = ArrayToModify[index];
  ArrayToModify.RemoveAt(index);
  return Value;
}

// FTransform AProjectCharacter::Retrieve(const TArray<FTransform>& ArrayToModify) 
// {
//   int32 Num = ArrayToModify.Num();
//   if (CurTBAIndex - 1 < 0) {
//     CurTBAIndex = Num - 1;
//   }
//   else {
//     CurTBAIndex --;
//   }
//   FTransform Value = ArrayToModify[CurTBAIndex];
//   ArrayToModify.RemoveAt(CurTBAIndex);
//   return Value;
// }

// FVector AProjectCharacter::Retrieve(const TArray<FVector>& ArrayToModify) 
// {
//   int32 Num = ArrayToModify.Num();
//   if (CurVBAIndex - 1 < 0) {
//     CurVBAIndex = Num - 1;
//   }
//   else {
//     CurVBAIndex --;
//   }
//   FVector Value = ArrayToModify[CurVBAIndex];
//   ArrayToModify.RemoveAt(CurVBAIndex);
//   return Value;
// }

FTransform AProjectCharacter::RetrieveTransform() 
{
  // return Retrieve(TransformBufferArr, CurTBAIndex);
  // TArray<FTransform>& ArrayToModify = TransformBufferArr;
  // int32& index = CurTimeIndex;
  // int32 Num = ArrayToModify.Num();
  // if (ensure(index >= 0 && index < Num)) {
  //   FTransform Value = ArrayToModify[index];
  //   ArrayToModify.RemoveAt(index);
  
  //   if (index - 1 < 0) {
  //     index = Num - 1;
  //   }
  //   else if (index == Num) {
  //     index = Num - 1;
  //   }
  //   else {
  //     index --;
  //   }
  //   return Value;
  // }
  // else {
  //   UE_LOG(LogTemp, Warning, TEXT("RABBITDUCK index is %i, num is %i"), index, Num);
  //   if (index - 1 < 0) {
  //     index = Num - 1;
  //   }
  //   else if (index == Num) {
  //     index = Num - 1;
  //   }
  //   else {
  //     index --;
  //   }
  // }
  
  // return FTransform(FVector(0));
  int32 Num = TransformBufferArr.Num();
  if (ensure(CurTimeIndex >= 0 && CurTimeIndex < Num)) {
    FTransform Value = TransformBufferArr[CurTimeIndex];
    Tindex = CurTimeIndex;
    return Value;
  }
  else if (Num > 0) {
    FTransform Value = TransformBufferArr[Num - 1];
    Tindex = Num - 1;
    UE_LOG(LogTemp, Warning, TEXT("RED FLAG"));
    if (ensure(false)) {

    }
  }
  return FTransform(FVector(0));
}

FVector AProjectCharacter::RetrieveVelocity() 
{
  // return Retrieve(VelocityBufferArr, CurVBAIndex);
  // TArray<FVector>& ArrayToModify = VelocityBufferArr;
  // int32& index = CurTimeIndex;
  // int32 Num = ArrayToModify.Num();
  // if (ensure(index >= 0 && index < Num)) {
  //   FVector Value = ArrayToModify[index];
  //   ArrayToModify.RemaoveAt(index);
  //   return Value;
  
  //   if (index - 1 < 0) {
  //     index = Num - 1;
  //   }
  //   else if (index == Num) {
  //     index = Num - 1;
  //   }
  //   else {
  //     index --;
  //   }
  //   return Value;
  // }
  
  // return FVector(0);

  int32 Num = VelocityBufferArr.Num();
  if (ensure(CurTimeIndex >= 0 && CurTimeIndex < Num)) {
    FVector Value = VelocityBufferArr[CurTimeIndex];
    return Value;
  }
  else if (Num > 0) {
    FVector Value = VelocityBufferArr[Num - 1];
    UE_LOG(LogTemp, Warning, TEXT("RED FLAG"));
    if (ensure(false)) {

    }
  }
  return FVector(0);
}

void AProjectCharacter::AdvanceTime() 
{
  if (CurTimeIndex + 1 > MaxTime) {
    CurTimeIndex = 0;
  }
  else {
    CurTimeIndex++;
  }
}

void AProjectCharacter::ReverseTime() 
{
  if (CurTimeIndex - 1 < 0) {
    CurTimeIndex = MaxTime;
  }
  else if (CurTimeIndex > MaxTime) {
    CurTimeIndex = MaxTime;
    if (ensure(false)) {
    }
  }
  else {
    CurTimeIndex--;
  }
}

bool AProjectCharacter::SaveCurValues() 
{
  TArray<FTransform> TransformBufferArrC(TransformBufferArr);
  TArray<FVector> VelocityBufferArrC(VelocityBufferArr);

  if (TransformBufferArr.Num() <= 0 || VelocityBufferArr.Num() <= 0) return false;

  FTransform CurTransform = TransformBufferArrC[CurTimeIndex];

  for (int32 i = 0; i < (TransformBufferArrC.Num() - 1) - CurTimeIndex; i++) {
    for (int32 Index = TransformBufferArrC.Num() - 1; Index > 0; Index--) {
      if (!ensure(Index < TransformBufferArrC.Num() - 1) || ensure(Index - 1 < 0)) return false;
      TransformBufferArrC[Index] = TransformBufferArrC[Index - 1];
    }
    if (ensure(TransformBufferArrC.Num() - 1 < 0)) return false;
    TransformBufferArrC[0] = TransformBufferArrC[TransformBufferArrC.Num() - 1];
  }
  FVector CurVelocity = VelocityBufferArrC[CurTimeIndex];
  for (int32 i = 0; i < (TransformBufferArrC.Num() - 1) - CurTimeIndex; i++) {
    for (int32 Index = VelocityBufferArrC.Num() - 1; Index > 0; Index--) {
      if (!ensure(Index < VelocityBufferArrC.Num() - 1) || ensure(Index - 1 < 0)) return false;
      VelocityBufferArrC[Index] = VelocityBufferArrC[Index - 1];
    }
    if (ensure(VelocityBufferArrC.Num() - 1 < 0)) return false;
    VelocityBufferArrC[0] = VelocityBufferArrC[VelocityBufferArrC.Num() - 1];
  }
  int32 TIndex = TransformsArr.Add(TransformBufferArrC);
  int32 VIndex = VelocitiesArr.Add(VelocityBufferArrC);

  if (!ensure(TIndex == VIndex)) {
  }
  FRewindInformation Info(TransformBufferArrC, VelocityBufferArrC);
  InfoArr.Add(Info);
  return true;

}

void AProjectCharacter::PauseRecording() 
{
  GetWorldTimerManager().PauseTimer(TimerHandle);
}

void AProjectCharacter::ResetRecordingAndTime() 
{
  ResetTime();
  ShiftDown(GetActorTransform(), GetVelocity());
  GetWorldTimerManager().UnPauseTimer(TimerHandle);
}


void AProjectCharacter::MoveForward(float Value) {
  if (Value == 0 || !ensure(GetController()))
    return;
  FRotator YawRotation(0, GetController()->GetControlRotation().Yaw, 0);

  FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

  AddMovementInput(ForwardVector, Value);
}

void AProjectCharacter::MoveRight(float Value) {
  if (Value == 0 || !ensure(GetController()))
    return;
  FRotator YawRotation(0, GetController()->GetControlRotation().Yaw, 0);

  FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  AddMovementInput(RightVector, Value);
}

void AProjectCharacter::OnHealthChanged(float Health, float MaxHealth, bool FullHealth, float PreviousHealth, AActor* EffectInstigator) {
  if (bIsDead)
    return;
  BP_OnHealthChanged(Health, MaxHealth, FullHealth, PreviousHealth, EffectInstigator);

  if (bShowDamageIndicatorOnHit) {
    if (Health < PreviousHealth) {
      if (HasAuthority()) {
        if (EffectInstigator) {
          UE_LOG(LogTemp, Warning, TEXT("Effect Instigator is: %s"), *EffectInstigator -> GetName());
          UActorComponent* Comp = EffectInstigator->GetComponentByClass(DICClass);
          UE_LOG(LogTemp, Warning, TEXT("FIrst"));
          if (Comp) {
            UE_LOG(LogTemp, Warning, TEXT("FIrst"));
            UDamageIndicatorComponent* DIComp = Cast<UDamageIndicatorComponent>(Comp);
            if (DIComp) {
              UE_LOG(LogTemp, Warning, TEXT("Second"));
              DIComp->AddDamageIndicatorText_CLIENT(FText::FromString(FString::FromInt(FMath::FloorToInt(PreviousHealth - Health))), GetActorLocation());
            }
          }
          LastDamagedBy = EffectInstigator;
        }
      }
    }
  }
  if (Health <= 0.f && PreviousHealth != Health) {
    bIsDead = true;
    BP_OnDeath();
  }
}

void AProjectCharacter::OnManaChanged(float Mana, float MaxMana) { BP_OnManaChanged(Mana, MaxMana); }

void AProjectCharacter::OnMainAttributeChanged(EAttributeType Type, float CurrentAttributeValue) { BP_OnMainAttributeChanged(Type, CurrentAttributeValue); }

void AProjectCharacter::SetTeamIDByControllerType() {
  if (!GetController())
    return;
  if (IsPlayerControlled()) {
    TeamID = 0;
  } else {
    TeamID = 1;
  }
}

void AProjectCharacter::OnRecord() {
  // if (BufferArr.Num() >= 100) {
  //   ShiftDown(GetActorTransform());
  // } else {
  //   TransformBufferArr.Add(GetActorTransform());
  //   VelocityBufferArr.Add(GetVelocity());
  // }

  // ShiftDownFloat(FMath::Rand());
  AdvanceTime();
  ShiftDown(GetActorTransform(), GetVelocity());
  CheckTime();
  // CircularBuffer
}


void AProjectCharacter::ShiftDown(FTransform TransformToSet, FVector VectorToSet) {
  // for (int32 Index = TransformBufferArr.Num() - 1; Index > 0; Index--) {
  //   TransformBufferArr[Index] = TransformBufferArr[Index - 1];
  // }
  // TransformBufferArr[0] = ValueToSet;

  // int32 num = VelocityBufferArr.Num() - 1 <= 100 ? VelocityBufferArr.Num() : 99;
  // for (int32 Index = VelocityBufferArr.Num() - 1; Index > 0; Index--) {
  //   VelocityBufferArr[Index] = VelocityBufferArr[Index - 1];
  // }
  // VelocityBufferArr[0] = VectorToSet;

  ShiftDownTransform(TransformToSet);
  ShiftDownVelocity(VectorToSet);


  
}

void AProjectCharacter::ShiftDownFloat(float ValueToSet) {
  int32 Range = FloatBufferArr.Num();
  if (Range < FBACapacity) {
    FloatBufferArr.Add(CurNumber);
    CurNumber++;
    return;
  }
  if (ensure(CurFBAIndex < Range && CurFBAIndex >= 0)) {
    FloatBufferArr[CurFBAIndex] = CurNumber;
    CurNumber++;
    UE_LOG(LogTemp, Warning, TEXT("The index is: %f"), CurFBAIndex);
  }
  if (CurFBAIndex + 1 >= Range) {
    CurFBAIndex = 0;
  }
  else {
    CurFBAIndex++;
  }
  

  // for (int32 Index = FloatBufferArr.Num() - 1; Index > 0; Index--) {
  //   FloatBufferArr[Index] = FloatBufferArr[Index - 1];
  // }
  // FloatBufferArr[0] = ValueToSet;
}

// template<class T>
// void AProjectCharacter::ShiftDown(const TArray<T>& ArrayToModify, T ValueToSet, int32& index) 
// {
//   int32 Range = FloatBufferArr.Num();
//   if (Range < FBACapacity) {
//     FloatBufferArr.Add(CurNumber);
//     CurNumber++;
//     return;
//   }
//   if (ensure(CurFBAIndex < Range && CurFBAIndex >= 0)) {
//     FloatBufferArr[CurFBAIndex] = CurNumber;
//     CurNumber++;
//     UE_LOG(LogTemp, Warning, TEXT("The index is: %f"), CurFBAIndex);
//   }
//   if (CurFBAIndex + 1 >= Range) {
//     CurFBAIndex = 0;
//   }
//   else {
//     CurFBAIndex++;
//   } 
// }

void AProjectCharacter::ShiftDownTransform(FTransform TransformToSet) 
{
  // int32 TNum = TransformBufferArr.Num();
  // if (TNum < TBACapacity) {
  //   int32 index = TransformBufferArr.Add(TransformToSet);
  //   CurTBAIndex = index;
  //   return;
  // }
  // if (CurTBAIndex + 1 >= TNum) {
  //   CurTBAIndex = 0;
  // }
  // else {
  //   CurTBAIndex++;
  // }
  // if (ensure(CurTBAIndex < TNum && CurTBAIndex >= 0)) {
  //   TransformBufferArr[CurTimeIndex] = TransformToSet;
  // }
  if (CurTimeIndex >= TransformBufferArr.Num()) {
     Tindex = TransformBufferArr.Add(TransformToSet);
  }
  else if(ensure(CurTimeIndex < TransformBufferArr.Num() && CurTimeIndex >= 0)) {
    TransformBufferArr[CurTimeIndex] = TransformToSet;
    Tindex = CurTimeIndex;
  }
}

void AProjectCharacter::ShiftDownVelocity(FVector VectorToSet) 
{
  // int32 VNum = VelocityBufferArr.Num();
  // if (VNum < VBACapacity) {
  //   int32 index = VelocityBufferArr.Add(VectorToSet);
  //   CurVBAIndex = index;
  //   return;
  // }
  // if (CurVBAIndex + 1 >= VNum) {
  //   CurVBAIndex = 0;
  // }
  // else {
  //   CurVBAIndex++;
  // }
  // if (ensure(CurVBAIndex < VNum && CurVBAIndex >= 0)) {
  //   VelocityBufferArr[CurVBAIndex] = VectorToSet;
  // }
  if (CurTimeIndex >= VelocityBufferArr.Num()) {
    VelocityBufferArr.Add(VectorToSet);
  }
  else if(ensure(CurTimeIndex < VelocityBufferArr.Num() && CurTimeIndex >= 0)) {
    VelocityBufferArr[CurTimeIndex] = VectorToSet;
  }
}
