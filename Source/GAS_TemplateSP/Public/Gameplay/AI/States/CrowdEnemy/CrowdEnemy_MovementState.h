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

protected:
	virtual void EvaluateAndStartMovement(TSharedPtr<FMovementStatePayload> Payload) override;

	void StartStrafing();

	UFUNCTION()
	void OnStrafingAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData);

	void OnWaitTimeFinished();

	virtual void OnExit_Implementation() override;

	UPROPERTY(EditDefaultsOnly)
	float MinStrafingWaitTime= 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float MaxStrafingWaitTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Instanced)
	UMovementSingleData* StrafingOrbitMovementData;

	UGAS_GameplayAbilityBase* LastUsedStrafingAbility;

private:
	FTimerHandle WaitForNextStrafingOrbitTimerHandle;

	class US_AICrowdEventManager* AICrowdEventManager;
};
