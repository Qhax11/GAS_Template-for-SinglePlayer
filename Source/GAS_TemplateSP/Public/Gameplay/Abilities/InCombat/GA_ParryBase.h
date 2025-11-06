// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TracePefromerOnMontage.h"
#include "Gameplay/StaticDelegates/S_DamageDelegates.h"
#include "GA_ParryBase.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UGA_ParryBase : public UGA_TracePefromerOnMontage
{
	GENERATED_BODY()
	
public:
	UGA_ParryBase();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	virtual void OnParryKnocbackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UFUNCTION()
	virtual void OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
