// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass, FName MontageSection = NAME_None);

	UFUNCTION(BlueprintCallable)
	float GetMaxRangeOfCurrentAttack();

protected:
	virtual void BeginPlay() override;

	virtual UGA_ComboMeleeAttack* ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag()) override;

	UFUNCTION()
	void OnEnemyCanActivateNextAttack();

	UFUNCTION()
	void OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData) override;
	
private:
	class AAIControllerBase* AIController;
	class UAC_TagDelegates* EnemyTagDelegatesComp;
	bool bOnTakeDamageState;
};
