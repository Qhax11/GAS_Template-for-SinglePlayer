// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/StaticDelegates/S_DamageDelegates.h"
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

class UAC_StateManager;
class UAC_BehaviorDecision;
class UComingAttackReactionData;
struct FComingAttackPayload;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_IntendHandlerBase : public UAC_AIControllerBase
{
	GENERATED_BODY()

public:	
	UAC_IntendHandlerBase();

protected:
	virtual void BeginPlay() override;

	virtual void OnHeroSpawned(const FHeroSpawnData& HeroSpawnData) override;

	UFUNCTION()
	void OnTargetDetected(AActor* DetectedTarget);

	UFUNCTION()
	void OnRequestEnemyBackupReaction();

	UPROPERTY()
	UAC_StateManager* OwnerStateManager;

	UPROPERTY()
	UAC_BehaviorDecision* OwnerBehaviorDecisionComp;

	US_DamageDelegates* DamageSubsystem;

	UFUNCTION()
	void OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTargetAbilityActivated(UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags);

	void SendEventToDefense(FComingAttackPayload& ComingAttackPayload);

	void TriggerIncomingAttackReaction(UComingAttackReactionData* Reaction, FComingAttackPayload Payload);

private:
	UFUNCTION()	
	void OnDamageDealt(const FDamageData& DamageData);

	virtual void RegisterTags();
};
