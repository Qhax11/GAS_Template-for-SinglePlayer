// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "GA_ComboMeleeAttackManager.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_ComboMeleeAttackManager : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

protected:

	UGA_ComboMeleeAttackManager();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	TSubclassOf<UGA_ComboMeleeAttack> GetNextComboMeleeAttackAbility();

	UFUNCTION()
	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	UFUNCTION()
	void OnCanExecuteNextAttack();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGA_ComboMeleeAttack>> ComboMeleeAttackAbilities;

	int32 AbilityIndex = 0;
};
