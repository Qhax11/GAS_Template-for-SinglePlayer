// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"

void UStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Enemy = StateInitParams.Enemy;
	EnemyController = StateInitParams.EnemyController;
	EnemyASC = StateInitParams.EnemyASC;
	EnemyTagDelegatesComp = StateInitParams.EnemyTagDelegatesComp;
	BehaviorDecisionComponent = StateInitParams.BehaviorDecisionComponent;
	HeroTarget = StateInitParams.HeroTarget;
	HeroTargetASC = StateInitParams.HeroTargetASC;
	StateManager = StateInitParams.StateManager;
}

void UStateBase::OnEnter_Implementation()
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[StateManager]: %s has been enter"), *StateTag.ToString());
	}
}

void UStateBase::OnExit_Implementation()
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return;
	}

	if (StateManager->bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[StateManager]: %s has been exit"), *StateTag.ToString());
	}
}

bool UStateBase::ExitRequest(FString Reason, const FGameplayTag& TransactionTag)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return false; 
	}

	return StateManager->RequestStateTreeExit(StateTag, TransactionTag, Reason);
}

bool UStateBase::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	if (!StateManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateManager is null in: %s"), *GetName());
		return false;
	}

	return StateManager->IsAttackInRange(AbilityClass);
}

FAttackData UStateBase::GetSelectedAttackAbilityData() const
{
	return StateManager->LastSelectedAttackData;
}
/*
FComingAttackReactionData UStateBase::GetSelectedReactionData() const
{
	return StateManager->SelectedReactionData;
}
*/
UGAS_GameplayAbilityBase* UStateBase::GetSelectedAttackAbilityCDO() const
{
	FAttackData SelectedAttackData = GetSelectedAttackAbilityData();
	return SelectedAttackData.AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
}

FAttackData UStateBase::SelectNewAttackAbility() const
{
	return StateManager->SelectNewBestAttack();
}

bool UStateBase::CheckThreadAndExitSafe(const FString& ExitReason)
{
	// The reason why this check is crucial is due to asynchronous tasks (Root Motion, Montage)
	// potentially calling delegates from Worker Threads.

	if (!IsInGameThread())
	{
		TWeakObjectPtr<UStateBase> WeakThis(this);
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([WeakThis, ExitReason]() // (capture)
				{
					if (UStateBase* Self = WeakThis.Get())
					{
						// Now running safely on the Game Thread.
						UE_LOG(LogTemp, Warning, TEXT("State Manager: Deferred ExitRequest(%s) executed on Game Thread."), *ExitReason);
						Self->ExitRequest(ExitReason);
					}
				}),
			TStatId(),
			nullptr,
			ENamedThreads::GameThread
		);
		return false;
	}

	// Zaten Game Thread'deyiz.
	UE_LOG(LogTemp, Warning, TEXT("State Manager: ExitRequest(%s) called directly on Game Thread."), *ExitReason);
	ExitRequest(ExitReason);
	return true; // Ýþlem hemen çaðrýldý.
}

