// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "PatrolingState.generated.h"

class UMovementSingleData;
class UAC_PatrolHandler;
class UCharacterMovementComponent;

UCLASS()
class GAS_TEMPLATESP_API UPatrolingState : public UStateBase
{
	GENERATED_BODY()

public:
	UPatrolingState();

	virtual void StateInitalize(const FStateInitParams& StateInitParams) override;

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnExit_Implementation() override;

protected:
	void OnPatrollingStopped();
	
	void StopEnemyMovement();

	UPROPERTY(EditDefaultsOnly)
	UMovementSingleData* MovementData;

	UPROPERTY()
	UAC_PatrolHandler* PatrolHandler;

	UPROPERTY()
	UCharacterMovementComponent* EnemyMovementComp;
};
