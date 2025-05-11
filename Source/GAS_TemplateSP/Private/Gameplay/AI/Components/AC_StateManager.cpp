// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"


UAC_StateManager::UAC_StateManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_StateManager::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_StateManager::StartStateByClass(AGAS_EnemyBase* EnemyBase, AAIControllerBase* EnemyController, TSubclassOf<UStateBase> StateClass)
{
	if (CurrentState)
	{
		CurrentState->OnExit(EnemyBase, EnemyController);
	}

	if (StateClass)
	{
		CurrentState = Cast<UStateBase>(NewObject<UObject>(this, StateClass));
		if (CurrentState)
		{
			CurrentState->OnEnter(EnemyBase, EnemyController);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to instantiate state class: %s"), *StateClass->GetName());
		}
	}
}

void UAC_StateManager::StopCurrentState()
{
}

bool UAC_StateManager::IsCurrentStateFinished() const
{
	return false;
}




