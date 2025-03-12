// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_ParryKnockbackBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_ParryKnockbackBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	UGA_ParryKnockbackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
