// Qhax's GAS Template for SinglePlayer

#pragma once

#include "AIController.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "AIControllerBase.generated.h"

class UAISenseConfig_Sight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetDetected, AActor*, DetectedTarget);

USTRUCT(BlueprintType)
struct FComingAttackPayload
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGAS_GameplayAbilityBase* ComingAttack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ComingAttackHitTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTagContainer ComingAttackTags;

	FComingAttackPayload()
		: ComingAttack(nullptr)
	{}

	FComingAttackPayload(UGAS_GameplayAbilityBase* InComingAttack, float InComingAttackHitTime, FGameplayTagContainer InComingAttackTags)
		: ComingAttack(InComingAttack), ComingAttackHitTime(InComingAttackHitTime), ComingAttackTags(InComingAttackTags)
	{}
};

UCLASS()
class GAS_TEMPLATESP_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE class UST_Base* GetStateTreeComponent() const { return StateTreeAIComponent; }

	FORCEINLINE class UAC_BehaviorDecision* GetBehaviorDecisionComponent() const { return BehaviorDecisionComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UST_Base> StateTreeAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAC_BehaviorDecision> BehaviorDecisionComponent;

protected:
	virtual void BeginPlay();

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	virtual void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// bHasTargetBeenDetected prevents multiple triggers of TargetPerceptionUpdated during each tick, ensuring it is only triggered once when the target is detected.
	bool bHasTargetBeenDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AGAS_HeroBase* TargetHero;

	UPROPERTY()
	AGAS_EnemyBase* ControlledEnemy;

public:
	UFUNCTION(BlueprintCallable)
	AGAS_HeroBase* GetTargetHero();

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();

	float GetTargetHeroDistance() const;

	FOnTargetDetected OnTargetDetected;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	// Listening own tags and target tag's
	virtual bool RegisterTags(AGAS_CharacterBase* TargetCharacter);

	UFUNCTION()
	void OnTargetAbilityActivated(UGameplayAbility* Ability);

	float GetAttackNotifyTriggerTime(class UGA_MeleeAttackBase* Ability, const FGameplayTagContainer& AbilityTags);

	void SendEventToDefense(FComingAttackPayload EventPayload);

	UFUNCTION()
	void OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnTakeDamageTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer TargetAbilityTagsCheck;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detaour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;

};
