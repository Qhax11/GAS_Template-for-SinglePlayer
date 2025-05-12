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

	void CreateStates();

	void EnterStateByClass(AGAS_EnemyBase* EnemyBase, AAIControllerBase* EnemyController, TSubclassOf<UStateBase> StateClass);

	void ExitStateByClass(AGAS_EnemyBase* EnemyBase, AAIControllerBase* EnemyController, TSubclassOf<UStateBase> StateClass);

	void RequestStateTreeExit(UStateBase* Requester);

	void StopCurrentState();
	bool IsCurrentStateFinished() const;


protected:
	UPROPERTY()
	UStateBase* CurrentState = nullptr;

	UPROPERTY()
	class UMovementState* MovementState;

	UPROPERTY()
	class UAttackState* AttackState;


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
