// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "SpecialAttackData.generated.h"


UCLASS()
class GAS_TEMPLATESP_API USpecialAttackData : public UAttackDataBase
{
	GENERATED_BODY()
	
public:
	USpecialAttackData();

	virtual bool IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug = nullptr) const override;

	virtual bool PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug = nullptr) const override;

	virtual float GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug = nullptr) const override;

	float GetMinRange() const override;

	float GetMaxRange() const override;

	virtual EAttackExecutionType GetExecutionType() const override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Ability class that defines the actual gameplay logic and range values"))
	TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "AI.Cooldown", ToolTip = "You have to choose spesific CooldownTag"))
	FGameplayTag AbilityCooldownTag;
};
