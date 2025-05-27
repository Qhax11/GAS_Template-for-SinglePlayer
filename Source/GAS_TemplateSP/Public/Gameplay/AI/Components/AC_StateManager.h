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

	void RequestStateTreeEnter(const FGameplayTag& StateTag);

	void RequestStateTreeExit(const FGameplayTag& StateTag, const FGameplayTag& TransactionTag);

	void ExitFromInComingAttackState(const FGameplayTag& TransactionTag);

	void ExitFromAttackState(const FGameplayTag& TransactionTag);

	void ExitFromMovementState(const FGameplayTag& TransactionTag);

	float GetTargetDistance() const;

	bool IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass);

	FComingAttackPayload ComingAttackPayload;
	bool bInComingAttack;

	FAttackData SelectNewBestAttack();
	FAttackData LastSelectedAttackData;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug = false;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "States")
	TArray<TSubclassOf<UStateBase>> StateClassArray;

	UPROPERTY()
	TArray<UStateBase*> StateInstances;

	UPROPERTY()
	UStateBase* CurrentState = nullptr;

	UPROPERTY()
	TObjectPtr<AGAS_EnemyBase> OwnerEnemy = nullptr;

	class AAIControllerBase* OwnerController;
	class AGAS_EnemyBase* OwnerEnemyBase;
	class UAC_BehaviorDecision* BehaviorDecisionComponent;
	class UST_Base* OwnerStateTree;
	class UGAS_AbilitySystemComponent* OwnerEnemyASC;
	class AGAS_HeroBase* HeroBase;
};
