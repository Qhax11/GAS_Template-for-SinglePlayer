// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "ComboChainAttackData.generated.h"

class UComboChainsAsset;

UCLASS()
class GAS_TEMPLATESP_API UComboChainAttackData : public UAttackDataBase
{
	GENERATED_BODY()

public:
	UComboChainAttackData();
	
	virtual bool IsEnable(const FAttackDecisionContext& Context, FAttackEnableDebug* OutDebug = nullptr) const override;

	virtual bool PassesChance(const FAttackDecisionContext& Context, FAttackChanceDebug* OutDebug = nullptr) const override;

	virtual float GetScore(const FAttackDecisionContext& Context, FAttackScoreDebug* OutDebug = nullptr) const override;

	virtual EAttackExecutionType GetExecutionType() const override;

public:
	UPROPERTY(EditDefaultsOnly)
	UComboChainsAsset* ComboChainAsset;
};
