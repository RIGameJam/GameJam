// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageIndicatorComponent.h"
#include "DamageIndicatorActor.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDamageIndicatorComponent::UDamageIndicatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* Owner = Cast<APawn>(GetOwner());
	PlayerController = Owner -> GetController<APlayerController>();
	// ...
	
}


// Called every frame
void UDamageIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Owner = Cast<APawn>(GetOwner());
	if (!Owner) return;
	if (Owner -> IsLocallyControlled()) return;
	if (!PlayerController) return;

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	if (ViewportSize.X <= 0.f || ViewportSize.Y <= 0.f) return;

	
	float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass()) -> GetDPIScaleBasedOnSize(ViewportSize.IntPoint());

	if (ViewportScale <= 0.f) return;

	FVector CameraLocation = PlayerController -> PlayerCameraManager -> GetCameraLocation();

	for (int32 i = 1; i < ActiveDIActors.Num(); ++i) {
		ADamageIndicatorActor* DIActor = ActiveDIActors[i];
		float Distance = FVector::Dist(CameraLocation, DIActor -> InitialLocation);

		float TextVerticalOffset = Distance / (ViewportSize.X / ViewportScale);
		DIActor -> SetActorLocation(DIActor -> InitialLocation + FVector(0, 0, i * TextVerticalOffset * TextSpacing));
	}
	
}

void UDamageIndicatorComponent::OnTextDestroyed(AActor* DestroyedActor) 
{
	ActiveDIActors.Pop();
}

void UDamageIndicatorComponent::AddDamageIndicatorText_CLIENT_Implementation(const FText& InText, const FVector& SpawnLocation) 
{
	if (!ensure(ActorClassToSpawn)) return;

	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation + FVector(0, 0, VerticalOffset));
	ADamageIndicatorActor* DIActor = GetWorld() -> SpawnActorDeferred<ADamageIndicatorActor>(ActorClassToSpawn, SpawnTransform, GetOwner());
	if (!ensure(DIActor)) return;

	DIActor -> Initialise(InText);
	DIActor -> OnDestroyed.AddDynamic(this, &UDamageIndicatorComponent::OnTextDestroyed);
	UGameplayStatics::FinishSpawningActor(DIActor, SpawnTransform);
	ActiveDIActors.Insert(DIActor, 0);

}

