// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_PostureRegen.h"

void UEC_PostureRegen::ExecuteWithParams(FExecCalculationParameters Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const float PostureRegenAmount = FMath::Max<float>(Params.GetSpec().GetSetByCallerMagnitude(GAS_Tags::TAG_Gameplay_EffectData_SetByCaller_PostureRegenAmount, .0f, true), .0f);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Params.GetTargetAttributeSet()->GetPostureAttribute(), EGameplayModOp::Additive, PostureRegenAmount));
}
