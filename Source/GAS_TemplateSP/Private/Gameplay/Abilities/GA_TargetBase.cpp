// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TargetBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

void UGA_TargetBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (BindInputForTargeting())
	{
		if (AGameplayAbilityTargetActor* TargetActor = SpawnAndSetupTargetActor())
		{
			WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, "None", ConfirmationType, TargetActor);
			WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnGameplayEventValidData);
			WaitTargetData->Cancelled.AddDynamic(this, &ThisClass::OnGameplayEventCancelled);
			WaitTargetData->Activate();
		}
	}
}

bool UGA_TargetBase::BindInputForTargeting()
{
	AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in %s. Cannot bind input for targeting because the Avatar Actor is not a valid hero."), *GetName());
		return false;
	}

	if (UAC_HeroControl* GetHeroControlComponent = HeroBase->GetHeroControlComponent())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = GetHeroControlComponent->GetEnhancedInputComponent())
		{
			if (GetHeroControlComponent->IA_ConfirmTarget && GetHeroControlComponent->IA_CancelTarget)
			{
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_ConfirmTarget, ETriggerEvent::Triggered, this, &UGA_TargetBase::ConfirmTargetingFromInput);
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_CancelTarget, ETriggerEvent::Triggered, this, &UGA_TargetBase::CancelAbilityFromInput);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
			}
		}
	}

	return false;
}

AGameplayAbilityTargetActor* UGA_TargetBase::SpawnAndSetupTargetActor()
{
	if (UWorld* World = this->GetWorld())
	{
		if (TargetActorClass->IsValidLowLevelFast())
		{
			FTransform ActorTransform(FRotator::ZeroRotator, FVector::ZeroVector);
			AGameplayAbilityTargetActor* TargetActor = World->SpawnActor<AGameplayAbilityTargetActor>(TargetActorClass, ActorTransform);

			return TargetActor;
		}
	}

	return nullptr;
}

void UGA_TargetBase::OnGameplayEventValidData(const FGameplayAbilityTargetDataHandle& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Target data received"));
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_TargetBase::OnGameplayEventCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Targeting cancelled"));
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
}

void UGA_TargetBase::ConfirmTargetingFromInput()
{
	if (WaitTargetData)
	{
		WaitTargetData->ExternalConfirm(true);
	}
}

void UGA_TargetBase::CancelAbilityFromInput()
{
	if (WaitTargetData)
	{
		WaitTargetData->ExternalCancel();
	}
}


