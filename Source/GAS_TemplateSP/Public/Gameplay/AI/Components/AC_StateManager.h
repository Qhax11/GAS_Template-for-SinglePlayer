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

	void StartStateByClass(AGAS_EnemyBase* EnemyBase, AAIControllerBase* EnemyController, TSubclassOf<UStateBase> StateClass);
	void StopCurrentState();
	bool IsCurrentStateFinished() const;

protected:
	UPROPERTY()
	UStateBase* CurrentState = nullptr;

	UPROPERTY()
	TObjectPtr<AGAS_EnemyBase> OwnerEnemy = nullptr;

};
