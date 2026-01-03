// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/InCombat/GA_ParryBase.h"
#include "GA_HeroParry.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroParry : public UGA_ParryBase
{
	GENERATED_BODY()
	
public:
	UGA_HeroParry();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	// TODO: U can make as AbilityTask so every ability can use easly and it will be blueprintable.
	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	UPROPERTY(EditDefaultsOnly, Category = "ParryKnocback")
	TSubclassOf<class UGA_ParryKnockbackBase> ParryKnockbackAbilityClass;

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	virtual void OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData) override;

	UFUNCTION()
	virtual void OnHeroInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UPROPERTY()
	class UAbilityTask_WaitInputRelease* WaitRelease;

	class UGAS_GameplayAbilityBase* ActivatedKnockbackAbility;

	virtual void OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
