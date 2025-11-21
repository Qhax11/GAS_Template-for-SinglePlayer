// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_EnemyDamage.h"

static const float PERFECT_DODGE_WINDOW = 0.25f;

void UEC_EnemyDamage::PreApplyDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const
{
    FGameplayTagContainer DodgeTags(GAS_Tags::TAG_Gameplay_State_Moving_Dash);

    FGameplayEffectQuery Query;
    Query.MakeQuery_MatchAnyOwningTags(DodgeTags);

    TArray<FActiveGameplayEffectHandle> ActiveEffects = Params.TargetASC->GetActiveGameplayEffects().GetActiveEffects(Query);
    if (ActiveEffects.Num() == 0)
    {
        return;
    }

    FActiveGameplayEffectHandle Handle = ActiveEffects[0];
    const FActiveGameplayEffect* ActiveEffect = Params.TargetASC->GetActiveGameplayEffect(Handle);
    if (!ActiveEffect)
    {
        return;
    }

    float DodgeStartTime = ActiveEffect->StartServerWorldTime;

    float CurrentTime = Params.TargetASC->GetWorld()->GetTimeSeconds();

    float TimeSinceDodge = CurrentTime - DodgeStartTime;
    if (TimeSinceDodge <= PERFECT_DODGE_WINDOW)
    {
        FGameplayEventData EventData;
        EventData.EventTag = GAS_Tags::TAG_Gameplay_Event_Trigger_Perfect_Dodge;

        // Listening from Dodge ability 
        if (Params.TargetASC)
        {
            Params.TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
        }

        DamageCalculationResult.DamageDealt = 0.0f;
    }
}
