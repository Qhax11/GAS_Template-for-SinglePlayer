// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"
#include "GA_MoveToLocation.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_MoveToLocation : public UGA_EnemyMovementBase
{
	GENERATED_BODY()
	
public:
	UGA_MoveToLocation();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

};
