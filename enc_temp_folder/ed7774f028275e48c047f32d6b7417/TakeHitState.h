// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "TakeHitState.generated.h"

class UGA_EnemyTakeDamage;

UCLASS()
class GAS_TEMPLATESP_API UTakeHitState : public UStateBase
{
	GENERATED_BODY()
public:
	UTakeHitState();

	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void ExecuteTakeHit(TSharedPtr<FTakeHitStatePayload> TakeHitPayload);

	void OnTakeHitAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	virtual void OnExit_Implementation() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGA_EnemyTakeDamage> EnemyTakeDamageAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedTakeDamageAbility;
};
