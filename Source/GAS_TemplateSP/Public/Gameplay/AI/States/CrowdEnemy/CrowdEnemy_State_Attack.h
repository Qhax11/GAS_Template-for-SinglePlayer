// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/States/AttackStateBase.h"
#include "CrowdEnemy_State_Attack.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UCrowdEnemy_State_Attack : public UAttackStateBase
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

	virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload) override;

	virtual void OnExit_Implementation() override;

protected:
	class US_AICrowdEventManager* AICrowdEventManager;
};
