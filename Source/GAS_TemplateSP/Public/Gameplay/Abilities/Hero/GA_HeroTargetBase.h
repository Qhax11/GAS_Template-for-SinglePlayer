// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_TargetBase.h"
#include "GA_HeroTargetBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroTargetBase : public UGA_TargetBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void OnTargetActorInitialized();

	bool BindInputForConfirmAndCancel();

	UFUNCTION()
	virtual void ConfirmTargetingFromInput();

	UFUNCTION()
	virtual void CancelAbilityFromInput();

};
