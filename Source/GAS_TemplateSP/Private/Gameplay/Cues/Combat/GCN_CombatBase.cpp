// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/GCN_CombatBase.h"
#include "AbilitySystemBlueprintLibrary.h"

bool AGCN_CombatBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Source || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: Source or Target is null!"));
		return false;
	}

	SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Source);
	TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!SourceASC || !TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceASC or TargetASC is null!"));
		return false;
	}

	SourceMeleeAttack = Cast<UGA_MeleeAttackBase>(Parameters.EffectContext.GetAbility());
	if (!SourceMeleeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceMeleeAttack is null!"));
		return false;
	}

	SourceMeleeAttackType = SourceMeleeAttack->GetAttackTypeTagFromAbilityTags();

	SourceDataComponent = Source->GetComponentByClass<UAC_GameplayData>();
	if (!SourceDataComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Gameplaycue: SourceDataComponent is null!"));
		return false;
	}

	return true;
}
