// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/AI/Components/AC_AIControllerBase.h"
#include "Gameplay/AI/DataTypes/Behavior/AttackData.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_StateManager.generated.h"

class UStateBase;
struct FStatePayloadBase;

struct FStateTransitionRequest
{
	FGameplayTag TargetStateTag;
	TSharedPtr<FStatePayloadBase> Payload;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_StateManager : public UAC_AIControllerBase
{
	GENERATED_BODY()

public:	
	UAC_StateManager();

	virtual void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	void OnAbilitySetGiven(const AActor* OwnerActor);

	void CreateStates();

	void StartLogic();

	UFUNCTION(BlueprintCallable)
	void StopLogic();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void HandleIncomingEvent(const FGameplayTag& StateEventTag, TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

	bool RequestStateTreeExit(const FStateTransitionRequest StateTransitionRequest, FString Reason);

	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug = false;

	UPROPERTY(BlueprintReadOnly)
	UStateBase* CurrentState = nullptr;

protected:
	bool RequestStateTreeEnter(const FGameplayTag& TargetStateTag, TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

	void HandleTargetDetected();

	void  DecideNextStateBasedOnAttackRange();

	// Find the instance of the requested state
	UStateBase* GetStateWithTag(const FGameplayTag& StateTag) const;

	bool bActive = true;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TArray<TSubclassOf<UStateBase>> StateClassArray;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.State"))
	FGameplayTag StartState;

	UPROPERTY()
	TArray<UStateBase*> StateInstances;

	UPROPERTY()
	class UAC_BehaviorDecision* BehaviorDecisionComponent;

	UPROPERTY()
	class UAC_TagDelegates* EnemyTagDelegatesComponent;

};
