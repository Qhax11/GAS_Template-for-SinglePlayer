// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GA_ComboManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_ComboManager : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	class UGAS_AbilitySystemComponent* ASC;

	UFUNCTION()
	void OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	void ActivateComboAttack();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UGAS_GameplayAbilityBase* LastActivatedCombo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class UGA_ComboMeleeAttack>> ComboAbilityClasses;

	int32 ComboIndex = 0;
};
