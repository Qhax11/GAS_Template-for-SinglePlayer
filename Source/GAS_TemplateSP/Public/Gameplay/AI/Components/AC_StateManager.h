// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/AI/DataTypes/State/StatePayloads.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_StateManager.generated.h"

class UStateBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_StateManager : public UAC_AIControllerBase
{
	GENERATED_BODY()

public:	
	UAC_StateManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StopLogic();

protected:
	UFUNCTION()
	void OnAbilitySetGiven(const AActor* OwnerActor);

	void CreateStates();

	void StartLogic();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

public:
	void HandleIncomingEvent(const FGameplayTag& StateEventTag, TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

protected:
	void HandleStateTransitionRequested(const FStateTransitionRequest& Request);

	void DecideNextStateBasedOnAttackRange();

	bool RequestStateTreeEnter(const FGameplayTag& TargetStateTag, TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

	// Find the instance of the requested state
	UStateBase* GetStateWithTag(const FGameplayTag& StateTag) const;

	bool bActive = true;

	UPROPERTY(BlueprintReadOnly)
	UStateBase* CurrentState = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TArray<TSubclassOf<UStateBase>> StateClassArray;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.State"))
	FGameplayTag StartState = GAS_Tags::TAG_AI_State_Patrolling;

	UPROPERTY()
	TArray<UStateBase*> StateInstances;

	UPROPERTY()
	class UAC_BehaviorDecision* BehaviorDecisionComponent;

	UPROPERTY()
	class UAC_TagDelegates* EnemyTagDelegatesComponent;

public:
	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug = false;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
