// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/ComboChainAttackData.h"

UComboChainAttackData::UComboChainAttackData()
{
}

bool UComboChainAttackData::IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug) const
{
	// If its fail in parent, fail here too
	if (!Super::IsEnable(Context, OutDebug))
	{
		return false;
	}

	return true;
}

bool UComboChainAttackData::PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug) const
{
	// If its fail in parent, fail here too
	if (!Super::PassesChance(Context, OutDebug))
	{
		return false;
	}

	return true;
}

float UComboChainAttackData::GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug) const
{
	float ParentScore = Super::GetScore(Context, OutDebug);
	return ParentScore;
}

EAttackExecutionType UComboChainAttackData::GetExecutionType() const
{
	return EAttackExecutionType::ComboChain;
}

float UComboChainAttackData::GetMinRange() const
{
	if (!ComboChainAsset || ComboChainAsset->ComboChain.ComboAbilities.Num() == 0)
	{
		return 0.f;
	}

	const FComboAbilityData& FirstStep = ComboChainAsset->ComboChain.ComboAbilities[0];
	if (!FirstStep.ComboAbilityClass)
	{
		return 0.f;
	}

	return FirstStep.ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>()->MinRange;
}

float UComboChainAttackData::GetMaxRange() const
{
	if (!ComboChainAsset || ComboChainAsset->ComboChain.ComboAbilities.Num() == 0)
	{
		return 0.f;
	}

	const FComboAbilityData& FirstStep = ComboChainAsset->ComboChain.ComboAbilities[0];
	if (!FirstStep.ComboAbilityClass)
	{
		return 0.f;
	}

	return FirstStep.ComboAbilityClass->GetDefaultObject<UGA_ComboMeleeAttack>()->MaxRange;
}
