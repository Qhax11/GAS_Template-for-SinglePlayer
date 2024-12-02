// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_MontageAbility.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "GA_MeleeAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATESP_API UGA_MeleeAttackBase : public UGA_MontageAbility
{
	GENERATED_BODY()

public:
	virtual void OnEventRecieved();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	TSubclassOf<UGameplayEffect> GEPhysicalDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
	FGameplayTagContainer TagsToAddToPhysicalDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
    FScalableFloat Damage;
};
