// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_EnemyDamage.h"

static const float PERFECT_DODGE_WINDOW = 0.25f;
static const float PERFECT_PARRY_WINDOW = 0.25f;

void UEC_EnemyDamage::PreApplyDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const
{
    /*
    FGameplayTagContainer DodgeTags(GAS_Tags::TAG_Gameplay_State_Moving_Dash);

    FGameplayEffectQuery QueryDash;
    QueryDash.MakeQuery_MatchAnyOwningTags(DodgeTags);

    TArray<FActiveGameplayEffectHandle> ActiveDashEffects = Params.TargetASC->GetActiveGameplayEffects().GetActiveEffects(QueryDash);
    if (ActiveDashEffects.Num() == 0)
    {
        return;
    }

    FActiveGameplayEffectHandle DashHandle = ActiveDashEffects[0];
    const FActiveGameplayEffect* ActiveDashEffect = Params.TargetASC->GetActiveGameplayEffect(DashHandle);
    if (!ActiveDashEffect)
    {
        return;
    }

    float DodgeStartTime = ActiveDashEffect->StartServerWorldTime;

    float CurrentTime = Params.TargetASC->GetWorld()->GetTimeSeconds();

    float TimeSinceDodge = CurrentTime - DodgeStartTime;
    if (TimeSinceDodge <= PERFECT_DODGE_WINDOW)
    {
        FGameplayEventData EventData;
        EventData.EventTag = GAS_Tags::TAG_Gameplay_Event_Trigger_Perfect_Dodge;

        // Listening from Dodge ability 
        if (Params.TargetASC)
        {
            //Params.TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
        }
        DamageCalculationResult.DamageDealt = 0.0f;
    }
    */
    /*
    FGameplayTagContainer ParryTags(GAS_Tags::TAG_Gameplay_State_InCombat_Parry);

    FGameplayEffectQuery QueryParry;
    QueryParry.MakeQuery_MatchAnyOwningTags(ParryTags);

    TArray<FActiveGameplayEffectHandle> ActiveEffects = Params.TargetASC->GetActiveGameplayEffects().GetActiveEffects(QueryParry);
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

    float ParryStartTime = ActiveEffect->StartServerWorldTime;

    float CurrentTime = Params.TargetASC->GetWorld()->GetTimeSeconds();

    float TimeSinceParry = CurrentTime - DodgeStartTime;
    if (TimeSinceParry <= PERFECT_PARRY_WINDOW)
    {
        FGameplayEventData EventData;
        EventData.EventTag = GAS_Tags::TAG_Gameplay_Event_Trigger_Perfect_Parry;

        // Listening from Dodge ability 
        if (Params.TargetASC)
        {
            Params.TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
        }

        DamageCalculationResult.DamageDealt = 0.0f;
    }
    */
}
