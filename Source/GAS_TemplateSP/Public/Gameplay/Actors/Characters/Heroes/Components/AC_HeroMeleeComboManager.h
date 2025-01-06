// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Components/AC_MeleeComboManager.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroMeleeComboManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_HeroMeleeComboManager : public UAC_MeleeComboManager
{
	GENERATED_BODY()

public:
	virtual void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None) override;

	void OnComboMeleeAttackInput();

protected:
	virtual void BeginPlay() override;

	bool BindHeroMeleeComboInput();

	AGAS_HeroBase* HeroBase;

public:

	UPROPERTY(EditDefaultsOnly, Category = "HeroMeleeComboManager|Input")
	const UInputAction* IA_ActivateMeleeCombo;

};
