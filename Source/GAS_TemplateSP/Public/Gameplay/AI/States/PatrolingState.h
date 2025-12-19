// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/StateBase.h"
#include "PatrolingState.generated.h"

class UAC_PatrolHandler;

UCLASS()
class GAS_TEMPLATESP_API UPatrolingState : public UStateBase
{
	GENERATED_BODY()

public:
	UPatrolingState();

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	void OnPatrollingStopped();
	
	virtual void OnExit_Implementation() override;

	UPROPERTY()
	UAC_PatrolHandler* PatrolHandler;
};
