// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRouterAttack.generated.h"

// Hero attack routing ability.
// Bound to primary attack input (LMB).
// Evaluates character state (dash, running, airborne, etc.) 
// and activates the appropriate attack ability.
// Falls back to the normal combo chain if no contextual state applies.

class AGAS_HeroBase;
class UAC_HeroMeleeComboManager;

UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRouterAttack : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_HeroRouterAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> RunAttackClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> DodgeRecoveryAttackClass;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	UAC_HeroMeleeComboManager* HeroMeleeComboManager;


};
