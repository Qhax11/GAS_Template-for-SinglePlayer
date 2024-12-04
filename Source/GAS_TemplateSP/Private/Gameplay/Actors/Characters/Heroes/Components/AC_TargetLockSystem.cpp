// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_TargetLockSystem.h"
#include "Gameplay/Abilities/Targeting/GAS_AbilityTargetingData.h"

UAC_TargetLockSystem::UAC_TargetLockSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_TargetLockSystem::StartTargetLock()
{
	if (!TargetingData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetingData is null in: %s"), *GetName());
		return;
	}

	TArray<AActor*> OutResultActors;
	TargetingData->Trace->CreateTraceFromTargetingDataWithTeamFilter(GetWorld(), OutResultActors, HeroBase, ETeamAttitude::Hostile);

	if (OutResultActors.IsValidIndex(0)) 
	{
		CurrentTarget = OutResultActors[0];
		bLocked = true;
	}
}

void UAC_TargetLockSystem::EndTargetLock()
{
	CurrentTarget = nullptr;
	bLocked = false;
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
	TryBindTargetLockSystemInputs(EnhancedInputComponent);
}

void UAC_TargetLockSystem::TryBindTargetLockSystemInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent is null in: %s"), *GetName());
		return;
	}

	if (ActivateTargetLockInput && LookMouseInput)
	{
		EnhancedInputComponent->BindAction(LookMouseInput, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::LookMouse);
		EnhancedInputComponent->BindAction(ActivateTargetLockInput, ETriggerEvent::Triggered, this, &UAC_TargetLockSystem::TryActivateTargetLock);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
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

void UAC_TargetLockSystem::TryActivateTargetLock(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("TryActivateTargetLock"));

	if (!bLocked) 
	{
		StartTargetLock();
	}
	else
	{
		EndTargetLock();
	}
}

void UAC_TargetLockSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

