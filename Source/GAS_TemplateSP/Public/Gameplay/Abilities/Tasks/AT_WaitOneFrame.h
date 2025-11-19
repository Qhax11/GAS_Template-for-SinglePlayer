// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitOneFrame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitOneFrameDelegate);

UCLASS()
class GAS_TEMPLATESP_API UAT_WaitOneFrame : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FWaitOneFrameDelegate OnFinished;

	// Static factory
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait One Frame", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAT_WaitOneFrame* WaitOneFrame(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

protected:
	bool bWaitedFrame = false;
};
