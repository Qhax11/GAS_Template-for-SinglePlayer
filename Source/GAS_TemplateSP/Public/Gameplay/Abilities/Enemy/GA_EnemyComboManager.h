// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "GA_EnemyComboManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyComboManager : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
public:
	UGA_EnemyComboManager();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	void ActivateComboMeleeAttackAbility();

	TSubclassOf<UGA_ComboMeleeAttack> GetNextComboMeleeAttackAbility();

	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGA_ComboMeleeAttack>> ComboMeleeAttackAbilities;

	int32 AbilityIndex = 0;

};
