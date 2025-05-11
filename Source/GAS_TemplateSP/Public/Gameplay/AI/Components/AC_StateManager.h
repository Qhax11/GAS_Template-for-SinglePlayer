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

	void StartStateByClass(TSubclassOf<UStateBase> StateClass);
	void StopCurrentState();
	bool IsCurrentStateFinished() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UStateBase* CurrentState = nullptr;

	UPROPERTY()
	TObjectPtr<AGAS_EnemyBase> OwnerEnemy = nullptr;

};
