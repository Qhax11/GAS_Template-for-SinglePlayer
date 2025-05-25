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

	virtual void OnExit() override;

protected:
	void SelectedAttack();

	void MakeAttack();

	UFUNCTION()
	void OnAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedAttack;

	void MakeComboAttack();

	UFUNCTION()
	void OnComboChaindEnded();

	void MakeShadowAttack();

	UFUNCTION()
	void ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData);

	UFUNCTION()
	void OnShadowAttackAbilityEnded(const FAbilityEndedDataBP& ShadowAttackAbilityEndedData);

	UGA_BossShadowAttack* LastUsedShadowAttack;
	TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackClass;
};
