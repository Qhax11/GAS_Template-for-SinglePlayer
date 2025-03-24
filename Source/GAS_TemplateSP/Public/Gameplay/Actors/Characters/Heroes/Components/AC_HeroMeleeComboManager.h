// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "AC_HeroMeleeComboManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_HeroMeleeComboManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_HeroMeleeComboManager();

	virtual void BeginPlay() override;

	bool BindHeroMeleeComboInput();

	void OnComboMeleeAttackInput();

	UFUNCTION(BlueprintCallable)
	void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None);

	TSubclassOf<UGA_ComboMeleeAttack> GetNextComboMeleeAttackAbility();

	void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	AGAS_HeroBase* HeroBase;
	UAbilitySystemComponent* HeroASC;

public:
	// The tags are blocks MeleeCombo because of same input
	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager")
	const UInputAction* IA_ActivateMeleeCombo;

protected:
	UFUNCTION()
	void OnCanActivateNextAttack();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGA_ComboMeleeAttack>> ComboMeleeAttackAbilities;

	int32 AbilityIndex = 0;

	bool bCanActivateAbility = true;
};
