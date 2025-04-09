// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroTargetBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"

void UGA_HeroTargetBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (BindInputForConfirmAndCancel())
	{
		if (bActorWillSpawnWithEQS)
		{
			StartEQSForTargetActorSpawnLocation();
		}
		else
		{
			SpawnAndSetupTargetActor();
		}
	}
}

bool UGA_HeroTargetBase::BindInputForConfirmAndCancel()
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
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_ConfirmTarget, ETriggerEvent::Triggered, this, &UGA_HeroTargetBase::ConfirmTargetingFromInput);
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_CancelTarget, ETriggerEvent::Triggered, this, &UGA_HeroTargetBase::CancelAbilityFromInput);
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

void UGA_HeroTargetBase::ConfirmTargetingFromInput()
{
	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	ApplyGameplayEffectToSelf(AbilityConfirmEffects);

	if (TargetActor->IsValidLowLevel())
	{
		TargetActor->Confirm();
	}
}

void UGA_HeroTargetBase::CancelAbilityFromInput()
{
	if (TargetActor->IsValidLowLevel())
	{
		TargetActor->Cancel();
	}
}
