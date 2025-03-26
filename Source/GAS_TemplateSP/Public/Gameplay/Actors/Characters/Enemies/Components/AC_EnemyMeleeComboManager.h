// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "AC_EnemyMeleeComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboEnded);

UCLASS()
class GAS_TEMPLATESP_API UAC_EnemyMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartComboChainWithClass(TSubclassOf<UGA_ComboMeleeAttack> ComboMeleeAttackAbilityClass);
	
protected:
	virtual void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None) override;

	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData) override;

	UPROPERTY(BlueprintAssignable)
	FOnComboEnded OnComboEnded;

	// Ranges of combo attack abilities.
	UPROPERTY(EditDefaultsOnly)
	TArray<float> ComboRanges;

private:
	class AAIControllerBase* AIController;

	float GetTargetDistance();
};
