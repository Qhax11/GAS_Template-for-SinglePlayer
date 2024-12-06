// Qhax's GAS Template for 2D SinglePlayer


#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Abilities/Tracing/GAS_AbilityTraceData.h"
#include "Kismet\KismetSystemLibrary.h"


void UGAS_GameplayAbilityBase::TraceForHostileUnits(TArray<AActor*>& OutActors)
{
	TraceData->Trace->CreateTraceWithTeamFilter(GetWorld(), GetAvatarActorFromActorInfo(), ETeamAttitude::Hostile, OutActors);
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



