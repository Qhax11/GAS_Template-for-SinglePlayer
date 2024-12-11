// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TargetBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"

UGA_TargetBase::UGA_TargetBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting);
}

void UGA_TargetBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	if (BindInputForConfirmAndCancel())
	{
		if (SpawnAndSetupTargetActor()) 
		{
			TargetActor->OnConfirm.AddDynamic(this, &UGA_TargetBase::OnTargetActorConfirm);
			TargetActor->OnCancel.AddDynamic(this, &UGA_TargetBase::OnTargetActorCancelled);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor cannot spawned in: %s"), *GetName());
			EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		}
	}
}

bool UGA_TargetBase::BindInputForConfirmAndCancel()
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

AGAS_TargetActorBase* UGA_TargetBase::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
	if (UWorld* World = this->GetWorld())
	{
		if (TargetActorClass->IsValidLowLevelFast())
		{
			FTransform ActorTransform = FTransform(Rotation, Location);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
			TargetActor = World->SpawnActor<AGAS_TargetActorBase>(TargetActorClass, ActorTransform, SpawnParams);
			return TargetActor;
		}
	}

	return nullptr;
}


void UGA_TargetBase::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
	TargetActor->Destroy();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_TargetBase::OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData)
{
	TargetActor->Destroy();
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
}

void UGA_TargetBase::ConfirmTargetingFromInput()
{
	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	StartupEffects();

	if (TargetActor)
	{
		TargetActor->Confirm();
	}
}

void UGA_TargetBase::CancelAbilityFromInput()
{
	if (TargetActor)
	{
		TargetActor->Cancel();
	}
}


