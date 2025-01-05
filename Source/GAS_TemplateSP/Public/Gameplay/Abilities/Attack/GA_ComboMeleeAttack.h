// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "GA_ComboMeleeAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCanExecuteNextAttack);

UCLASS()
class GAS_TEMPLATESP_API UGA_ComboMeleeAttack : public UGA_MeleeAttackBase
{
	GENERATED_BODY()

public:
	UGA_ComboMeleeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	FCanExecuteNextAttack OnCanExecuteNextAttack;

protected:
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

};
