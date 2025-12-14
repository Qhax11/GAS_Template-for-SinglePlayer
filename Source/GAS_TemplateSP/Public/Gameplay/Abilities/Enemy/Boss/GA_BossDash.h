// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GA_DodgeBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "GA_BossDash.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_BossDash : public UGA_DodgeBase
{
	GENERATED_BODY()
	
protected:
	UGA_BossDash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual FVector CalculateDestination() override;

	FGameplayTag DirectionTag;
};
