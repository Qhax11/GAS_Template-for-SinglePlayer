// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Effects/Calculations/EC_DamageBase.h"
#include "EC_HeroDamage.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UEC_HeroDamage : public UEC_DamageBase
{
	GENERATED_BODY()
	
public:
	virtual void PostCalculateDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const override;

};
