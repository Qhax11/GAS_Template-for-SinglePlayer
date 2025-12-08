// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/AttackStateBase.h"
#include "Boss_State_Attack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UBoss_State_Attack : public UAttackStateBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual bool SelectAndMakeAttack(TSharedPtr<FAttackStatePayload> AttackStatePayload) override;

	void MakeShadowAttack();

	UFUNCTION()
	void ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData);

	UFUNCTION()
	void OnShadowAttackAbilityEnded(const FCustomAbilityEndedData& ShadowAttackAbilityEndedData);

	class UGA_BossShadowAttack* LastUsedShadowAttack;

	virtual void OnExit_Implementation() override;
};
