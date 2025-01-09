// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "GA_TakeDamageBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_TakeDamageBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:

	UGA_TakeDamageBase();

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UAnimMontage* GetHitMontage(UAnimMontage* AttackMontage);

	void SetRotationToInstigator(const AActor* Instigator);

	UPROPERTY(EditDefaultsOnly)
	TMap<UAnimMontage*, UAnimMontage*> AttackAndHitMontages;
};
