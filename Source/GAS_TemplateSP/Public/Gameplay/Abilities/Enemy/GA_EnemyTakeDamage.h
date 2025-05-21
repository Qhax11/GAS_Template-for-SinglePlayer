// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "GA_EnemyTakeDamage.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyTakeDamage : public UGA_TakeDamageBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void SetRotationToInstigator(const AActor* Instigator);

};
