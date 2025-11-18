// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_PostureDecrease.h"

void UEC_PostureDecrease::ExecuteWithParams(FExecCalculationParameters Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const float PostureDecreaseAmount = FMath::Max<float>(Params.GetSpec().GetSetByCallerMagnitude(GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_PostureDecreaseAmount, .0f, true), .0f);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Params.GetTargetAttributeSet()->GetPostureAttribute(), EGameplayModOp::Additive, -PostureDecreaseAmount));
}
