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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void OnMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Knocback")
	TObjectPtr<UAnimMontage> KnocbackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Knocback")
	TSubclassOf<UGameplayEffect> ParryKnockbackEffect;

	UFUNCTION()
	virtual void OnKnocbackMontageMontageBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnKnocbackMontageMontageInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnKnocbackMontageMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnKnocbackMontageMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnKnocbackMontageMontageEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	// TODO: U can make as AbilityTask so every ability can use easly and it will be blueprintable.
	UFUNCTION()
	void OnDamageDealt(const FDamageData& DamageData);

	FName TaskName;

	UCurveFloat* CurveFloat;

	class UGAS_Task_PlayMontageWaitForEvent* PlayMontageKnocback;

	UFUNCTION()
	void OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
};
