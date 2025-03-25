// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboEnded);

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ActivateComboMeleeAttackAbilityWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass);

	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData) override;

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;
};
