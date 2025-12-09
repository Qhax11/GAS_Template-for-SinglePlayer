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

	void OnComboAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData) override;

	void ChangeComboSet();

public:
	virtual UGA_ComboMeleeAttack* ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None, FGameplayTag AdditionalTag = FGameplayTag()) override;

	UFUNCTION(BlueprintCallable)
	void StartShadowCombo(FName MontageSection, FGameplayTag AdditionalTag = FGameplayTag());

	AGAS_HeroBase* HeroBase;

private:
	UFUNCTION()
	void OnInAirTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnInAirTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnPhaseActiveHitTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

};
