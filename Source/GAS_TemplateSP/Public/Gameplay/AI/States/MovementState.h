// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
#include "MovementState.generated.h"

struct FMovementStatePayload : public FStatePayloadBase
{
	UMovementChainAsset* MovementChainAsset = nullptr;

	TSubclassOf<UGAS_GameplayAbilityBase> TargetAttackClass = nullptr; // Range check için

	FMovementStatePayload(UMovementChainAsset* InMovementChainAsset, TSubclassOf<UGAS_GameplayAbilityBase> InTargetAttackClass)
		: MovementChainAsset(InMovementChainAsset), TargetAttackClass(InTargetAttackClass) {
	}
};

UCLASS()
class GAS_TEMPLATESP_API UMovementState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnExit_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();

	bool IsInRangeForAttack() const;

	void StartMovementChain(TSharedPtr<FMovementStatePayload> MovementStatePayload);

	UFUNCTION()
	void OnMovementChainEnded();

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;

	UGAS_GameplayAbilityBase* SelectedAttackCDO;
};
