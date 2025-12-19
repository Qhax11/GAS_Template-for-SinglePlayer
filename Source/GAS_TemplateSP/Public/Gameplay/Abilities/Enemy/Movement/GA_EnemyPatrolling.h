// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/Enemy/Movement/GA_EnemyMovementBase.h"
#include "GA_EnemyPatrolling.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyPatrolling : public UGA_EnemyMovementBase
{
	GENERATED_BODY()
	
public:
	UGA_EnemyPatrolling();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void OnMoveCompleted();

	UFUNCTION()
	void OnMoveAborted();

	UFUNCTION()
	void OnMoveFailed();
};
