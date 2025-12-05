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
class UAC_StateManager;
class UAC_SequenceExecutor;
class UAC_BehaviorDecision;
class UST_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetDetected, AActor*, DetectedTarget);


UCLASS()
class GAS_TEMPLATESP_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UST_Base* GetStateTreeComponent() const { return StateTreeAIComponent; }

	FORCEINLINE UAC_BehaviorDecision* GetBehaviorDecisionComponent() const { return BehaviorDecisionComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAC_StateManager* GetEnemyStateManagerComponent() const { return EnemyStateManagerComponent; }

	FORCEINLINE UAC_SequenceExecutor* GetSequenceExecutorComponent() const { return SequenceExecutorComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UST_Base> StateTreeAIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAC_BehaviorDecision> BehaviorDecisionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UAC_StateManager* EnemyStateManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UAC_SequenceExecutor* SequenceExecutorComponent;

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void UpdateRotationTowardsTarget(float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 10.f;

	UFUNCTION()
	virtual void TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// bHasTargetBeenDetected prevents multiple triggers of TargetPerceptionUpdated during each tick, ensuring it is only triggered once when the target is detected.
	bool bHasTargetBeenDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AGAS_HeroBase* TargetHero;

	UPROPERTY()
	AGAS_EnemyBase* ControlledEnemy;

	UPROPERTY()
	UAbilitySystemComponent* ControlledEnemyASC;

public:
	UFUNCTION(BlueprintCallable)
	AGAS_HeroBase* GetTargetHero();

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();

	float GetTargetHeroDistance() const;

	FOnTargetDetected OnTargetDetected;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detaour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;

};
