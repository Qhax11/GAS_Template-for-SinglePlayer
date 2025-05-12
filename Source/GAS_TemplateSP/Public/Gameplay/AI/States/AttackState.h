// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "AttackState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAttackState : public UStateBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter(AGAS_EnemyBase* OwnerEnemy, AAIControllerBase* OwnerController);

	void ExecuteSelectedAttack();

	void ExecuteAttack();

	void ExecuteComboAttack();

	void ExecuteShadowAttack();

	void OnAttackAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	UFUNCTION()
	void OnComboChaindEnded();
};
