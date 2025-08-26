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

	virtual UGA_ComboMeleeAttack* ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag()) override;

	void OnOwnerAbilityEnd(const FAbilityEndedData& EndedData) override;

	UFUNCTION()
	void OnCanActivateNextAttack();

public:
	UFUNCTION(BlueprintCallable)
	void ActivateComboMeleeAttackAbilityWithShadowAttack(FName MontageSection, int32 ComboIndex = 0, FGameplayTag AdditionalTag = FGameplayTag());

	// The tags are blocks MeleeCombo because of same input
	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	const UInputAction* IA_ActivateMeleeCombo;

	AGAS_HeroBase* HeroBase;
};
