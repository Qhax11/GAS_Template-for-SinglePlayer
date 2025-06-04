// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_IntendHandlerBase.generated.h"

/**
 * UAC_IntendHandlerBase
 *
 * Base component class responsible for listening to AI signals and intents.
 * Derived classes like UAC_IntendHandlerBoss and UAC_IntendHandlerCrowd
 * implement specific behaviors for different AI types.
 *
 * It processes external stimuli and triggers state transitions via the State Manager.
 */


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_IntendHandlerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_IntendHandlerBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetDetected(AActor* DetectedTarget);

	UFUNCTION()
	void OnRequestEnemyBackupReaction();

	UPROPERTY()
	class AAIControllerBase* OwnerController;

	UPROPERTY()
	class UAC_StateManager* OwnerStateManager;

	UPROPERTY()
	class UAC_BehaviorDecision* OwnerBehaviorDecisionComp;

	UPROPERTY()
	AGAS_EnemyBase* ControlledEnemy;

	UPROPERTY()
	class AGAS_HeroBase* TargetHero;

	// Listening own tags and target tag's
	virtual bool RegisterTags(AGAS_CharacterBase* TargetCharacter);

	UFUNCTION()
	void OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTargetAbilityActivated(UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags);

	void SendEventToDefense(FComingAttackPayload EventPayload);

	void TriggerIncomingAttackReaction(struct FComingAttackReactionData Reaction, FComingAttackPayload Payload);
};
