// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroMeleeComboManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_HeroMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	bool BindHeroMeleeComboInput();

	void OnComboMeleeAttackInput();

	virtual UGA_ComboMeleeAttack* ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None) override;

	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData) override;

	UFUNCTION()
	void OnCanActivateNextAttack();

	// The tags are blocks MeleeCombo because of same input
	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	const UInputAction* IA_ActivateMeleeCombo;

	AGAS_HeroBase* HeroBase;
};
