// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Hero/GA_HeroTargetBase.h"
#include "GA_HeroShadowAttack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroShadowAttack : public UGA_HeroTargetBase
{
	GENERATED_BODY()

protected:
	UGA_HeroShadowAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
	virtual void SpawnAndSetupTargetActor(FRotator Rotation = FRotator::ZeroRotator, FVector Location = FVector::ZeroVector) override;

	void SetShadowToShadowController();

	virtual void OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData) override;

	UFUNCTION()
	void OnTargetLockedTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UPROPERTY()
	class UAC_TagDelegates* HeroTagDelegatesComp;
};
