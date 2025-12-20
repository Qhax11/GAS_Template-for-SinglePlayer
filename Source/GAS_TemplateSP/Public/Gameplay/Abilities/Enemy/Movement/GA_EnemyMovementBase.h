// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GA_EnemyMovementBase.generated.h"

class UAT_AIMoveTo;

UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyMovementBase : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UGA_EnemyMovementBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	void ExecuteMoveTask(UAT_AIMoveTo* MoveTask);

	UFUNCTION()
	virtual void OnMoveCompleted();

	UFUNCTION()
	virtual void OnMoveAborted();

	UFUNCTION()
	virtual void OnMoveFailed();

	UFUNCTION()
	virtual void OnExpectedDurationFinished() { return; }

	UFUNCTION()
	virtual void OnMinDurationFinished() { return; }

private:
	/**
	 * Failsafe handler for movement timeout.
	 * Called when the movement task exceeds its maximum allowed duration.
	 *
	 * This is a safety mechanism to prevent infinite or stuck movement tasks.
	 * It is NOT part of movement behavior design and must not be overridden.
	 * Always ends the ability.
	 */
	UFUNCTION()
	void HandleMaxDurationReached();

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementBase")
	float AcceptanceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementBase")
	float MovementSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementBase")
	float MinMovementDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyMovementBase")
	float MaxMovementDuration = 5.0f;

	UPROPERTY()
	AGAS_EnemyBase* EnemyCharacter;

	UPROPERTY()
	AAIControllerBase* EnemyController;

	UPROPERTY()
	UCharacterMovementComponent* EnemyMovementComp;

private:
	float CachedExpectedDuration;

};
