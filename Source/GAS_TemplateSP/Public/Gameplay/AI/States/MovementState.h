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

class UAC_EnemyMovementManager;

UCLASS()
class GAS_TEMPLATESP_API UMovementState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void ExecuteMovement(TSharedPtr<FMovementStatePayload> MovementStatePayload);

	void StartMovementChain(UMovementChainAsset* MovementChain);

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();

	bool IsInRangeForAttack() const;

	UFUNCTION()
	void OnMovementChainEnded();

	virtual void OnExit_Implementation() override;

protected:
	TSharedPtr<FMovementStatePayload> MovementStateEnterPayload;

	UPROPERTY()
	UAC_EnemyMovementManager* MovementManagerComponent;

	UPROPERTY()
	UGAS_GameplayAbilityBase* SelectedAttackCDO;
};
