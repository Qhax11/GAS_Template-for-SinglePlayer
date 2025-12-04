// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"

#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Components/AC_AbilitySet.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/States/StateBase.h"


UAC_StateManager::UAC_StateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_StateManager::BeginPlay()
{
	Super::BeginPlay();

	if (!OwnerController || !OwnerEnemyBase || !OwnerEnemyASC || !HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Some owner variables are already set in: %s !"), *GetName());
		return;
	}

	OwnerEnemyBase->GetAbilitySetComponent()->OnAbilitySetGiven.AddDynamic(this, &UAC_StateManager::OnAbilitySetGiven);

	BehaviorDecisionComponent = OwnerController->GetBehaviorDecisionComponent();
	if (!BehaviorDecisionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorDecisionComponent is null in: %s"), *GetName());
		return;
	}

	EnemyTagDelegatesComponent = OwnerEnemyBase->GetTagDelegatesComponent();
	if (!EnemyTagDelegatesComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyTagDelegatesComponent is null in: %s !"), *GetName());
		return;
	}
} 

void UAC_StateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentState)
	{
		CurrentState->OnExit();
	}

	Super::EndPlay(EndPlayReason);
}

void UAC_StateManager::OnAbilitySetGiven(const AActor* OwnerActor)
{
	CreateStates();
	StartLogic();
}

void UAC_StateManager::CreateStates()
{
	UGAS_AbilitySystemComponent* TargetASC = nullptr;
	if (OwnerController->GetTargetHero())
	{
		TargetASC = Cast<UGAS_AbilitySystemComponent>(OwnerController->GetTargetHero()->GetAbilitySystemComponent());
	}

	FStateInitParams StateInitParams;
	StateInitParams.Enemy = OwnerEnemyBase;
	StateInitParams.EnemyController = OwnerController;
	StateInitParams.EnemyASC = OwnerEnemyASC;
	StateInitParams.EnemyTagDelegatesComp = EnemyTagDelegatesComponent;
	StateInitParams.BehaviorDecisionComponent = BehaviorDecisionComponent;
	StateInitParams.HeroTarget = OwnerController->GetTargetActor();
	StateInitParams.HeroTargetASC = TargetASC;
	StateInitParams.StateManager = this;

	for (TSubclassOf<UStateBase> StateClass : StateClassArray)
	{
		if (!*StateClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid state class in array."));
			continue;
		}

		UStateBase* NewState = NewObject<UStateBase>(this, StateClass);
		if (!NewState || !NewState->StateTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to instantiate: %s"), *StateClass->GetName());
			continue;
		}

		NewState->StateInitalize(StateInitParams);
		StateInstances.Add(NewState);
	}
}

void UAC_StateManager::StartLogic()
{
	RequestStateTreeEnter(StartState);
}

void UAC_StateManager::StopLogic()
{
	bActive = false;
}

void UAC_StateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->OnTick(DeltaTime);
#if WITH_EDITOR
		if (bEnableDebug) 
		{
			const FVector Location = OwnerEnemyBase->GetActorLocation() + FVector(0.f, 0.f, 150.f);
			const FString DebugText = FString::Printf(TEXT("State: %s"), *CurrentState->GetName());
			DrawDebugString(GetWorld(), Location, DebugText, nullptr, FColor::Cyan, 0.f, true, 1.5f);
		}
#endif // WITH_EDITOR
	}
}

void UAC_StateManager::HandleIncomingEvent(const FGameplayTag& StateEventTag, TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!StateEventTag.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: StateEventTag is empty!"));
		return;
	}

	if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_VulnerableTagAdded) 
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Vulnerable, EnterPayload);
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_InComingAttack) 
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_InComingAttack, EnterPayload);
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_TargetDetected) 
	{
		HandleTargetDetected();
	}
	else if (StateEventTag == GAS_Tags::TAG_AI_StateEvent_BackupReaction)
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_BackupReaction, EnterPayload);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: Unhandled StateEventTag: %s"), *StateEventTag.ToString());
	}

}

void UAC_StateManager::HandleTargetDetected()
{
	if (!CurrentState) 
	{
		return;
	}

	if (CurrentState->StateTag == GAS_Tags::TAG_AI_State_InComingAttack) 
	{
		return;
	}

	FAttackData NewSelectedAttack = SelectNewBestAttack();
	if (IsAttackInRange(NewSelectedAttack.AbilityClass))
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Attack);
	}
	else
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Movement);
	}
}

bool UAC_StateManager::RequestStateTreeEnter(const FGameplayTag& StateTag, TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return false;
	}

	if (bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s state has been requested to enter"), *StateTag.ToString());
	}

	UStateBase* FindedState = GetStateWithTag(StateTag);
	if (!IsValid(FindedState)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s FindedState is null!"));
		return false;
	}

	if (FindedState->EnterCondition(EnterPayload))
	{
		if (CurrentState)
		{
			CurrentState->OnExit();
		}

		FindedState->OnEnter(EnterPayload);
		CurrentState = FindedState;
		return true;
	}
	else
	{
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("[State Manager]: Condition of %s is false, cannot enter"), *FindedState->GetName());
		}
		return false;
	}
}

bool UAC_StateManager::RequestStateTreeExit(const FGameplayTag& StateTag, const FGameplayTag& TransactionTag, FString Reason)
{
	if (!StateTag.IsValid() || !bActive)
	{
		return false;
	}

	if (bEnableDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s state has been requested to exit, reason is: %s"), *StateTag.ToString(), *Reason);
	}

	UStateBase* FindedState = GetStateWithTag(StateTag);
	if (!FindedState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: FindedState is null!"), *FindedState->GetName());
		return false;
	}

	if (!IsCurrentState(StateTag))
	{
		return false;
	}

	if (!FindedState->ExitCondition()) 
	{
		if (bEnableDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("[State Manager]: Condition of %s is false, cannot exit"), *StateTag.ToString());
		}
		return false;
	}

	// If request coming with trancastion tag we directly enter
	if (TransactionTag.IsValid()) 
	{
		RequestStateTreeEnter(TransactionTag);
		return true;
	}

	HandleStateExit(StateTag);
	return true;
}

void UAC_StateManager::HandleStateExit(const FGameplayTag& ExitedState)
{
	FAttackData NewSelectedAttack = SelectNewBestAttack();
	if (IsAttackInRange(NewSelectedAttack.AbilityClass))
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Attack);
	}
	else
	{
		RequestStateTreeEnter(GAS_Tags::TAG_AI_State_Movement);
	}
}

bool UAC_StateManager::IsCurrentState(const FGameplayTag& StateTag)
{
	if (!StateTag.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: StateTag is null!"));
		return false;
	}

	UStateBase* FindedState = GetStateWithTag(StateTag);
	if (!FindedState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: FindedState is null!"), *StateTag.ToString());
		return false;
	}

	if (FindedState != CurrentState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[State Manager]: %s FindedState is not current state!"), *StateTag.ToString());
		return false;
	}

	return true;
}

float UAC_StateManager::GetTargetDistance() const
{
	if (!OwnerController)
	{
		return -1.0f;
	}

	return OwnerController->GetTargetHeroDistance();
}

bool UAC_StateManager::IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass)
{
	if (!AbilityClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Class is null in: %s"), *GetName());
		return false;
	}

	UGAS_GameplayAbilityBase* AbilityCDO = AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	if (AbilityCDO->MaxRange > GetTargetDistance() && AbilityCDO->MinRange < GetTargetDistance())
	{
		return true;
	}

	return false;
}

FAttackData UAC_StateManager::SelectNewBestAttack()
{
	FAttackData NewAttackData = BehaviorDecisionComponent->GetBestAttack();
	LastSelectedAttackData = NewAttackData;
	return NewAttackData;
}

UStateBase* UAC_StateManager::GetStateWithTag(const FGameplayTag& StateTag) const
{
	if (!StateTag.IsValid()) 
	{
		return nullptr;
	}

	// Find the instance of the requested state
	for (UStateBase* State : StateInstances)
	{
		if (State && State->StateTag == StateTag)
		{
			return State;
		}
	}

	return nullptr;
}






