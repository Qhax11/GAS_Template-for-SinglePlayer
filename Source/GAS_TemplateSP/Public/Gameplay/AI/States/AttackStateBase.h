// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AttackStateBase.generated.h"

class UGAS_GameplayAbilityBase;

UCLASS()
class GAS_TEMPLATESP_API UAttackStateBase : public UStateBase
{
	GENERATED_BODY()
	
public:
	UAttackStateBase();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter_Implementation() override;

protected:
	virtual bool SelectAndMakeAttack();

	void MakeAttack();

	UFUNCTION()
	void OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedAttack;

	TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass;

public:
	virtual void OnExit_Implementation() override;

};
