// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_ParryBase.h"
#include "Gameplay/StaticDelegates/S_DamageDelegates.h"
#include "GA_HeroParry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroParry : public UGA_ParryBase
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly, Category = "ParryKnocback")
	TSubclassOf<class UGA_ParryKnockbackBase> ParryKnockbackAbilityClass;

	// TODO: U can make as AbilityTask so every ability can use easly and it will be blueprintable.
	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	UFUNCTION()
	void OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UFUNCTION()
	void OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
