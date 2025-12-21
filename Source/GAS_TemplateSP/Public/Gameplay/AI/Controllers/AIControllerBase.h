// Qhax's GAS Template for SinglePlayer

#pragma once

#include "AIController.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "AIControllerBase.generated.h"

class UAC_BehaviorDecision;
class UAC_IntendManager;
class UAC_StateManager;
class UAISenseConfig_Sight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetDetected, AActor*, DetectedTarget);


UCLASS()
class GAS_TEMPLATESP_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UAC_BehaviorDecision* GetBehaviorDecisionComponent() const { return BehaviorDecisionComponent; }

	FORCEINLINE UAC_IntendManager* GetIntendManagerComponent() const { return EnemyIntendManagerComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAC_StateManager* GetEnemyStateManagerComponent() const { return EnemyStateManagerComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Brain")
	TObjectPtr<UAC_BehaviorDecision> BehaviorDecisionComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components|Brain")
	UAC_IntendManager* EnemyIntendManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UAC_StateManager* EnemyStateManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

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
