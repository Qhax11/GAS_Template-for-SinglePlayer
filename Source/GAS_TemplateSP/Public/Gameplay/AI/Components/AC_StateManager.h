// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/DataTypes/CombatTypes.h"
#include "AC_StateManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_StateManager : public UActorComponent
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

	bool RequestStateTreeExit(const FGameplayTag& StateTag, const FGameplayTag& TransactionTag, FString Reason);

	bool IsCurrentState(const FGameplayTag& StateTag);

	float GetTargetDistance() const;

	bool IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass);

	FAttackData SelectNewBestAttack();
	FAttackData LastSelectedAttackData;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug = false;

	UPROPERTY(BlueprintReadOnly)
	UStateBase* CurrentState = nullptr;

protected:
	bool RequestStateTreeEnter(const FGameplayTag& StateTag, TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

	void HandleStateExit(const FGameplayTag& ExitedState);

	void HandleTargetDetected();

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
	TObjectPtr<AGAS_EnemyBase> OwnerEnemy = nullptr;

	UPROPERTY()
	class AAIControllerBase* OwnerController;

	UPROPERTY()
	class AGAS_EnemyBase* OwnerEnemyBase;

	UPROPERTY()
	class UAC_BehaviorDecision* BehaviorDecisionComponent;

	UPROPERTY()
	class UAC_TagDelegates* EnemyTagDelegatesComponent;

	UPROPERTY()
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;

	UPROPERTY()
	class AGAS_HeroBase* HeroBase;
};
