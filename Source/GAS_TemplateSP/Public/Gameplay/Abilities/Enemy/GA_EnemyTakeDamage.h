// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/GA_TakeDamageBase.h"
#include "GA_EnemyTakeDamage.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyTakeDamage : public UGA_TakeDamageBase
{
	GENERATED_BODY()
	
public:
	UGA_EnemyTakeDamage();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void SetRotationToInstigator(const AActor* Instigator);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
