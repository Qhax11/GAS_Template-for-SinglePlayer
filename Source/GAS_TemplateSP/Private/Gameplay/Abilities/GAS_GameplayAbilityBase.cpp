// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"

void UGAS_GameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	StartupEffects();
}

void UGAS_GameplayAbilityBase::StartupEffects()
{
	ApplyGameplayEffectToSelf(AbilityActivationEffects);
}

void UGAS_GameplayAbilityBase::ApplyGameplayEffectToSelf(TArray<TSubclassOf<UGameplayEffect>> Effects)
{
	UGAS_EffectBlueprintFunctionLibary::ApplyEffectArrayToTarget(GetAbilitySystemComponentFromActorInfo(), GetAbilitySystemComponentFromActorInfo(), Effects);
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

bool UGAS_GameplayAbilityBase::IsOnCooldown(UAbilitySystemComponent* ASC)
{
	if (!ASC) 
	{
		return false;
	}

	const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	return CooldownTags && ASC->HasAnyMatchingGameplayTags(*CooldownTags);
}

void UGAS_GameplayAbilityBase::IncreaseLevel(UAbilitySystemComponent* AbilitySystemComp)
{
	if (!AbilitySystemComp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComp is null in %s."), *GetName());
		return;
	}

	// Using the CDO here because "FindAbilitySpecFromClass" returns the CDO if called immediately after granting the ability.
	UGAS_GameplayAbilityBase* CDO_AbilityBase = Cast<UGAS_GameplayAbilityBase>(GetClass()->GetDefaultObject());
	FGameplayAbilitySpec* CDO_AbilitySpec = AbilitySystemComp->FindAbilitySpecFromClass(CDO_AbilityBase->GetClass());
	if (!CDO_AbilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySpec is null in %s."), *GetName());
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

	if (GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		// We broadcast the event through the CDO, ensuring that even binders without access to the instance are triggered
		UGAS_GameplayAbilityBase* CDO_AbilityBase = Cast<UGAS_GameplayAbilityBase>(GetClass()->GetDefaultObject());
		CDO_AbilityBase->OnGameplayAbilityEndedWithDataBP.Broadcast(FAbilityEndedDataBP(this, bWasCancelled));
	}
	else
	{
		OnGameplayAbilityEndedWithDataBP.Broadcast(FAbilityEndedDataBP(this, bWasCancelled));
	}
}

