// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Effects/Calculations/EC_HeroDamage.h"
#include "Gameplay/Attributes/AS_Hero.h"

void UEC_HeroDamage::PostCalculateDamageResult(FExecCalculationParameters& Params, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, FDamageCalculationResult& DamageCalculationResult) const
{
    if (!DamageCalculationResult.bParrySuccess)
    {
        const UAS_Hero* HeroAttributes = Cast<UAS_Hero>(Params.GetSourceAttributeSet());
        if (!HeroAttributes)
        {
            UE_LOG(LogTemp, Error, TEXT("BaseAttributes is NULL!"));
            return;
        }

        float DamageDealt = DamageCalculationResult.DamageDealt;

        FGameplayAttribute ShadowGaugeAttribute = HeroAttributes->GetShadowGaugeAttribute();
        if (!ShadowGaugeAttribute.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("ShadowGauge Attribute is INVALID!"));
            return;
        }

        UGameplayEffect* GE_IncreaseShadowGuage = UGAS_EffectBlueprintFunctionLibary::CreateInstantEffectWithModifier(
            ShadowGaugeAttribute, EGameplayModOp::Additive, DamageDealt);

        Params.SourceASC->ApplyGameplayEffectToSelf(GE_IncreaseShadowGuage, 1, FGameplayEffectContextHandle());
    }
}
