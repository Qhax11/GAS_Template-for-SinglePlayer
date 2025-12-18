// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"

UGAS_GameplayAbilityBase::UGAS_GameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Debuff_Stun);
}

void UGAS_GameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bApplyCommit) 
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
	}

	CharacterBase = Cast<AGAS_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGAS_GameplayAbilityBase: CharacterBase is null in: %s"), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	StartupEffects();
	RemoveTags();
}

void UGAS_GameplayAbilityBase::StartupEffects()
{
	ApplyGameplayEffectToSelf(ActivationEffectsToApply);
}

void UGAS_GameplayAbilityBase::ApplyGameplayEffectToSelf(TArray<TSubclassOf<UGameplayEffect>> Effects)
{
	// Use the library function to apply effects and capture only the Handles of non-instant effects.
	ActiveEffectsOnOwner = UGAS_EffectBlueprintFunctionLibary::ApplyEffectArrayToTarget(GetAbilitySystemComponentFromActorInfo(), this, GetAbilitySystemComponentFromActorInfo(), Effects);
}

void UGAS_GameplayAbilityBase::RemoveTags()
{
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (OwnerASC)
	{
		for (const FGameplayTag& Tag : TagsToRemove)
		{
			OwnerASC->RemoveLooseGameplayTag(Tag, 100);
		}
	}
}

float UGAS_GameplayAbilityBase::GetCost(int32 AbilityLevel) const
{
	UGameplayEffect* CostGameplayEffect = GetCostGameplayEffect();
	float CostValue = 0;

	if (CostGameplayEffect && CostGameplayEffect->Modifiers.IsValidIndex(0))
	{
		// Getting cost value from CostGameplayEffect
		CostGameplayEffect->Modifiers[0].ModifierMagnitude.GetStaticMagnitudeIfPossible(AbilityLevel, CostValue);
	}

	return CostValue;
}

float UGAS_GameplayAbilityBase::GetCoolDown(int32 AbilityLevel) const
{
	UGameplayEffect* CooldownGameplayEffect = GetCooldownGameplayEffect();
	float CooldownDuration = 0;

	if (CooldownGameplayEffect && CooldownGameplayEffect->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		// Getting cooldown duration from CooldownGameplayEffect
		CooldownGameplayEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(AbilityLevel, CooldownDuration);
	}

	return CooldownDuration;
}

void UGAS_GameplayAbilityBase::EndAbilityManually()
{
	if (IsActive())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
	}
}

void UGAS_GameplayAbilityBase::IncreaseLevel(UAbilitySystemComponent* AbilitySystemComp)
{
	if (!AbilitySystemComp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGAS_GameplayAbilityBase: AbilitySystemComp is null in %s."), *GetName());
		return;
	}

	// Using the CDO here because "FindAbilitySpecFromClass" returns the CDO if called immediately after granting the ability.
	UGAS_GameplayAbilityBase* CDO_AbilityBase = Cast<UGAS_GameplayAbilityBase>(GetClass()->GetDefaultObject());
	FGameplayAbilitySpec* CDO_AbilitySpec = AbilitySystemComp->FindAbilitySpecFromClass(CDO_AbilityBase->GetClass());
	if (!CDO_AbilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability: UGAS_GameplayAbilityBase: AbilitySpec is null in %s."), *GetName());
		return;
	}

	int32 NewAbilityLevel = ++CDO_AbilitySpec->Level;

	// This is actual setting.
	CDO_AbilitySpec->Level = NewAbilityLevel;

	CDO_AbilityBase->OnAbilityLevelChanged.Broadcast(this, NewAbilityLevel);

	float NewCost = GetCost(NewAbilityLevel);
	CDO_AbilityBase->OnAbilityCostChanged.Broadcast(this, NewCost);

	float NewCooldown = GetCoolDown(NewAbilityLevel);
	CDO_AbilityBase->OnAbilityCooldownChanged.Broadcast(this, NewCooldown);
}

void UGAS_GameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	for (const FActiveGameplayEffectHandle& HandleToRemove : ActiveEffectsOnOwner)
	{
		if (HandleToRemove.IsValid()) 
		{
			GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(HandleToRemove);
		}
	}

	// DIRECT BROADCAST — no timer
	if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		UGAS_GameplayAbilityBase* CDO = Cast<UGAS_GameplayAbilityBase>(GetClass()->GetDefaultObject());
		if (CDO)
		{
			CDO->OnAbilityEnded.Broadcast(FCustomAbilityEndedData(this, bWasCancelled));
		}
	}
	else
	{
		OnAbilityEnded.Broadcast(FCustomAbilityEndedData(this, bWasCancelled));
	}
}

