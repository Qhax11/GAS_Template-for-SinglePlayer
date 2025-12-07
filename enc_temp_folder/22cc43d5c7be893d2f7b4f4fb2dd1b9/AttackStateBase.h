// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "AttackStateBase.generated.h"

class UGAS_GameplayAbilityBase;

struct FAttackStateStatePayload : public FStatePayloadBase
{
	FAttackData AttackData;

	FAttackStateStatePayload(FAttackData IAttackData)
		: AttackData(IAttackData) {
	}

	FAttackStateStatePayload() = default;
};

UCLASS()
class GAS_TEMPLATESP_API UAttackStateBase : public UStateBase
{
	GENERATED_BODY()
	
public:
	UAttackStateBase();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

protected:
	virtual bool SelectAndMakeAttack(TSharedPtr<FAttackStateStatePayload> AttackStatePayload);

	void MakeAttack(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass);

	UFUNCTION()
	void OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	UGAS_GameplayAbilityBase* LastUsedAttack;

public:
	virtual void OnExit_Implementation() override;

};
