// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/MovementState.h"
#include "CrowdEnemy_MovementState.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UCrowdEnemy_MovementState : public UMovementState
{
	GENERATED_BODY()

public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnExit_Implementation() override;

	virtual void OnTick_Implementation(float DeltaTime) override;

	void TryEnterToAttackState();
	
	void SelectMovement();

	void StartMovementChain(TSubclassOf<class UGAS_GameplayAbilityBase> SelectedAttackAbilityClass);

	void OnMovementChainEnded();

	void MakeStrafingAbility();

	UFUNCTION()
	void OnStrafingAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	void OnWaitTimeFinished();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGAS_GameplayAbilityBase> StrafingAbilityClass;

	UGAS_GameplayAbilityBase* LastUsedStrafingAbility;

	UPROPERTY(EditDefaultsOnly)
	FGameplayEventData StrafingAbilityEventData;

	UPROPERTY(EditDefaultsOnly)
	float MinStrafingWaitTime= 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float MaxStrafingWaitTime = 3.0f;

private:
	class UAC_EnemyMovementManager* MovementManagerComponent;
	FTimerHandle WaitForNextStrafingOrbitTimerHandle;

	class US_AICrowdEventManager* AICrowdEventManager;
};
