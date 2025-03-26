// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "AC_MeleeComboManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboMeleeAbilityEnded, const bool, bWasCancelled);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_MeleeComboManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_MeleeComboManager();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateComboMeleeAttackAbility(FName MontageSection = NAME_None);

protected:
	virtual void BeginPlay() override;

	class AGAS_CharacterBase* CharacterBase;
	UAbilitySystemComponent* CharacterBaseASC;

	TSubclassOf<UGA_ComboMeleeAttack> GetNextComboMeleeAttackAbility();

	UFUNCTION()
	virtual void OnComboMeleeAttackAbilityEnd(const FAbilityEndedData& EndedData);

	UFUNCTION()
	void OnCanActivateNextAttack();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGA_ComboMeleeAttack>> ComboMeleeAttackAbilities;

	int32 AbilityIndex = 0;

	bool bCanActivateAbility = true;
};
