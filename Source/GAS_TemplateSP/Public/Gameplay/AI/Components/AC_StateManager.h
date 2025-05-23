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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void CreateStates();

	void EnterStateByClass(TSubclassOf<UStateBase> StateClass);

	void ExitStateByClass(TSubclassOf<UStateBase> StateClass);

	void RequestStateTreeExit(UStateBase* Requester);

	void ExitFromInComingAttackState();

	void ExitFromAttackState();

	void StopCurrentState();
	bool IsCurrentStateFinished() const;

	float GetTargetDistance() const;

	bool IsInRange();

	FComingAttackPayload ComingAttackPayload;
	bool bInComingAttack;

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
