// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"
#include "GA_ComboMeleeAttack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_ComboMeleeAttack : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

public:
	UGA_ComboMeleeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

protected:
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

};
