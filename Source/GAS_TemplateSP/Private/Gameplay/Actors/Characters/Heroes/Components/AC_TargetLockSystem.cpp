// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"

UAC_TargetLockSystem::UAC_TargetLockSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_TargetLockSystem::BeginPlay()
{
	Super::BeginPlay();

	HeroBase = Cast<AGAS_HeroBase>(GetOwner());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s)"), *GetName());
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(HeroBase->InputComponent);
	TryBindLookMouseInputs(EnhancedInputComponent);
}

void UAC_TargetLockSystem::TryBindLookMouseInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (LookMouseInputAction)
	{
		EnhancedInputComponent->BindAction(LookMouseInputAction, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::LookMouse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LookMouseInputAction action are null in: %s"), *GetName());
	}
}

void UAC_TargetLockSystem::LookMouse(const FInputActionValue& Value)
{
	// Input is a Vector2D
	const FVector2D VectorValue = Value.Get<FVector2D>();

	if (VectorValue.X > Threshold)
	{
		TargetChange(true);
	}

	if (VectorValue.X < -Threshold)
	{
		TargetChange(false);
	}
}

void UAC_TargetLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

