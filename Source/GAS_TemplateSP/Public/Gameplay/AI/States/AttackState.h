// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"
#include "AttackState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAttackState : public UStateBase
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter() override;

	void SelectedAttack();

	void MakeAttack();

	void MakeComboAttack();

	void MakeShadowAttack();

	UFUNCTION()
	void OnShadowAttackAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData);

	UFUNCTION()
	void ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData);

	UFUNCTION()
	void OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UFUNCTION()
	void OnComboChaindEnded();

protected:
	TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackClass;
};
