// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/GCN_CombatBase.h"

void AGCN_CombatBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Source || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: Source or Target is null!"));
		return;
	}

	SourceMeleeAttack = Cast<UGA_MeleeAttackBase>(Parameters.EffectContext.GetAbility());
	if (!SourceMeleeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceMeleeAttack is null!"));
		return;
	}

	SourceMeleeAttackType = SourceMeleeAttack->GetAttackTypeTagFromAbilityTags();
	if (!SourceMeleeAttackType.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceMeleeAttackType is null!"));
		return;
	}

	SourceDataComponent = Source->GetComponentByClass<UAC_GameplayData>();
	if (!SourceDataComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceDataComponent is null!"));
		return;
	}


}
