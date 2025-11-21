// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_EnemyDamage.h"

static const float PERFECT_DODGE_WINDOW = 0.25f;

void UEC_EnemyDamage::PreApplyDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const
{
    // GAS Tag Container oluþtur (Aradýðýmýz Tag)
    FGameplayTagContainer DodgeTags(GAS_Tags::TAG_Gameplay_State_Moving_Dash);

    // 1. Hedefin ASC'sinde bu Tag'e sahip AKTÝF Effect'leri sorgula
    FGameplayEffectQuery Query;
    Query.MakeQuery_MatchAnyOwningTags(DodgeTags);

    // Sorguya uyan Effect'lerin listesini al
    TArray<FActiveGameplayEffectHandle> ActiveEffects = Params.TargetASC->GetActiveGameplayEffects().GetActiveEffects(Query);

    // Eðer en az bir tane Dodge Effect'i varsa (Genelde 1 tanedir)
    if (ActiveEffects.Num() > 0)
    {
        // Ýlk bulunan effect handle'ýný al
        FActiveGameplayEffectHandle Handle = ActiveEffects[0];

        // Handle üzerinden gerçek Effect verisine ulaþ
        const FActiveGameplayEffect* ActiveEffect = Params.TargetASC->GetActiveGameplayEffect(Handle);

        if (ActiveEffect)
        {
            // 2. Effect'in baþlama zamanýný al
            float DodgeStartTime = ActiveEffect->StartServerWorldTime;

            // Þu anki zamaný al
            float CurrentTime = Params.TargetASC->GetWorld()->GetTimeSeconds();

            // 3. Delta Hesapla
            float TimeSinceDodge = CurrentTime - DodgeStartTime;

            // 4. Karar Aný
            if (TimeSinceDodge <= PERFECT_DODGE_WINDOW)
            {
                // FGameplayEventData'yý hazýrlayýn
                FGameplayEventData EventData;
                EventData.EventTag = GAS_Tags::TAG_Gameplay_Event_Trigger_Perfect_Dodge; // Dinlediðiniz tag

                // Hedef ASC üzerinden olayý gönderin (TargetASC, Dodge yapan karakterin ASC'sidir)
                if (Params.TargetASC)
                {
                    Params.TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
                }
            }
            else
            {
                // --- NORMAL DODGE (Hasar yok ama Perfect deðil) ---
                DamageCalculationResult.MitigatedDamage = 0.0f;
            }
        }
    }
    // Eðer Tag yoksa veya Effect bulunamazsa zaten hasar normal iþler.
}
