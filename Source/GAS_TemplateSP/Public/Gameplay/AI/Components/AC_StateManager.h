// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/AI/States/StateBase.h"
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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void EnterStateByClass(TSubclassOf<UStateBase> StateClass);

	void ExitStateByClass(TSubclassOf<UStateBase> StateClass);

	void RequestStateTreeEnter(UStateBase* Requester, const FGameplayTag& TransactionTag);

	void RequestStateTreeExit(UStateBase* Requester, const FGameplayTag& TransactionTag);

	void ExitFromInComingAttackState(const FGameplayTag& TransactionTag);

	void ExitFromAttackState(const FGameplayTag& TransactionTag);

	void ExitFromMovementState(const FGameplayTag& TransactionTag);

	float GetTargetDistance() const;

	bool IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass);

	FComingAttackPayload ComingAttackPayload;
	bool bInComingAttack;

	FAttackData SelectNewBestAttack();
	FAttackData LastSelectedAttackData;

protected:
	UPROPERTY()
	UStateBase* CurrentState = nullptr;

	UPROPERTY()
	class UMovementState* MovementState;

	UPROPERTY()
	class UAttackState* AttackState;

	UPROPERTY()
	class UInComingAttackState* InComingAttackState;

	UPROPERTY()
	TObjectPtr<AGAS_EnemyBase> OwnerEnemy = nullptr;

	UPROPERTY()
	TArray<UStateBase*> StateInstances;

	class AAIControllerBase* OwnerController;
	class AGAS_EnemyBase* OwnerEnemyBase;
	class UAC_BehaviorDecision* BehaviorDecisionComponent;
	class UST_Base* OwnerStateTree;
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;
	class AGAS_HeroBase* HeroBase;
};
