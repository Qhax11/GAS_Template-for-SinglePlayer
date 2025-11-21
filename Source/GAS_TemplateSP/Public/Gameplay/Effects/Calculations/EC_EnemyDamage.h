// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Effects/Calculations/EC_DamageBase.h"
#include "EC_EnemyDamage.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UEC_EnemyDamage : public UEC_DamageBase
{
	GENERATED_BODY()

public:
	virtual void PreApplyDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const override;
	
};
