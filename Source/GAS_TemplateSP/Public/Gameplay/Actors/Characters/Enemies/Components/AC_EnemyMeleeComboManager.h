// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass, FName MontageSection = NAME_None);

	virtual void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None) override;

protected:
	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData) override;
	
private:
	class AAIControllerBase* AIController;

	float GetTargetDistance();
};
