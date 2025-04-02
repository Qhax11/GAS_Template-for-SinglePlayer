// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GA_EnemyMovementBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_EnemyMovementBase : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UGA_EnemyMovementBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void RequestMoveToLocation(const FVector& MoveLocation);

	virtual void RequestMoveToTarget(AActor* TargetActor);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	FDelegateHandle MoveCompleteHandle;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 200.0f;

	AGAS_EnemyBase* EnemyCharacter;
	AAIControllerBase* EnemyController;
	UCharacterMovementComponent* EnemyMovementComp;
};
