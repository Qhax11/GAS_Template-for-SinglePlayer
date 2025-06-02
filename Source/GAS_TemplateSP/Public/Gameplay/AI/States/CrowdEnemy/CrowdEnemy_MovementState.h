// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "CrowdEnemy_MovementState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UCrowdEnemy_MovementState : public UStateBase
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter_Implementation() override;

	virtual void OnExit_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();
	
	void SelectMovement();

	void StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass);

	UFUNCTION()
	void OnMovementChainEnded();

	void MakeStrafingAbility();

	UFUNCTION()
	void OnStrafingAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData);

	FAttackData SelectedAttack;
	UGAS_GameplayAbilityBase* SelectedAttackCDO;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> StrafingAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedStrafingAbility;

	UPROPERTY(EditDefaultsOnly)
	FGameplayEventData StrafingAbilityEventData;

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;
};
