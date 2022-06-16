// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageIndicatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEJAMIDEA1_API UDamageIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageIndicatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void AddDamageIndicatorText_CLIENT(const FText& InText, const FVector& SpawnLocation);

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADamageIndicatorActor> ActorClassToSpawn = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float VerticalOffset;
	UPROPERTY(EditDefaultsOnly)
	float TextSpacing;
	UFUNCTION()
	void OnTextDestroyed(AActor* DestroyedActor);
	TArray<class ADamageIndicatorActor*> ActiveDIActors;

	APlayerController* PlayerController = nullptr;
		
};
