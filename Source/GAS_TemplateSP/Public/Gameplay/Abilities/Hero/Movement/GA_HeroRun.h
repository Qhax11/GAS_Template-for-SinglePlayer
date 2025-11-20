// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_HeroRun.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroRun : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_HeroRun(); 

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_SpeedBoostClass;

	FActiveGameplayEffectHandle GE_SpeedBoostHandle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_PostureDecreaseClass;

	FActiveGameplayEffectHandle GE_PostureDecreaseHandle;

	UPROPERTY(EditDefaultsOnly)
	float PostureDecreaseAmount = 1.f;

	UPROPERTY()
	class AGAS_HeroBase* Hero;

	UPROPERTY()
	class UAC_TagDelegates* HeroTagDelegatesComp;

	UFUNCTION()
	void OnPostureEmptyTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
