// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effects/Calculations/EC_Base.h"
#include "EC_PostureRegen.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UEC_PostureRegen : public UEC_Base
{
	GENERATED_BODY()

public:
	virtual void ExecuteWithParams(FExecCalculationParameters Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
