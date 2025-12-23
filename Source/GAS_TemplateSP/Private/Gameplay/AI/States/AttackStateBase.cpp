// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/ComboChainAttackData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/SpecialAttackData.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

UAttackStateBase::UAttackStateBase()
{
	StateTag = GAS_Tags::TAG_AI_State_Attack;
}

void UAttackStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
	ComboManager = Enemy->GetEnemyMeleeComboManagerComponent();
	check(ComboManager);
}

bool UAttackStateBase::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!EnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	TSharedPtr<FAttackStatePayload> AttackStatePayload = StaticCastSharedPtr<FAttackStatePayload>(EnterPayload);
	if (!AttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: AttackStatePayload is invalid in: %s"), *GetName());
		return false;
	}

	UAttackDataBase* AttackData = AttackStatePayload->AttackData;
	if (!AttackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: AttackData invalid in: %s"), *GetName());
		return false;
	}

	if (!IsValid(Enemy) || !IsValid(HeroTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: Enemy or HeroTarget is invalid in: %s"), *GetName());
		return false;
	}

	return true;
}

void UAttackStateBase::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	TSharedPtr<FAttackStatePayload> AttackStatePayload = StaticCastSharedPtr<FAttackStatePayload>(EnterPayload);
	check(AttackStatePayload.IsValid());

	ExecuteAttack(AttackStatePayload->AttackData);
}

void UAttackStateBase::ExecuteAttack(UAttackDataBase* SelectedAttackData)
{
	if (!SelectedAttackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: SelectedAttackData is null in: %s"), *GetName());
		return;
	}

	const EAttackExecutionType AttackExecutionType = SelectedAttackData->GetExecutionType();
	UE_LOG(LogTemp, Log, TEXT("State: UAttackStateBase: AttackExecutionType is: %s"), *UEnum::GetValueAsString(AttackExecutionType));

	if (AttackExecutionType == EAttackExecutionType::ComboChain)
	{
		ExecuteComboAttack(Cast<UComboChainAttackData>(SelectedAttackData));
	}
	else if (AttackExecutionType == EAttackExecutionType::Single)
	{
		ExecuteSpecialAttack(Cast<USpecialAttackData>(SelectedAttackData));
	}
}

void UAttackStateBase::ExecuteComboAttack(UComboChainAttackData* ComboData)
{
	if (!ComboManager || !ComboData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase: ComboManager or ComboData is null in: %s"), *GetName());
		return;
	}

	ComboManager->OnEnemyComboChainEnded.RemoveAll(this);
	ComboManager->OnEnemyComboChainEnded.AddUObject(this, &UAttackStateBase::OnComboEnd);

	ComboManager->StartComboChain(ComboData->ComboChainAsset);
}

void UAttackStateBase::ExecuteSpecialAttack(USpecialAttackData* SpecialAttackData)
{
	if (!SpecialAttackData || !SpecialAttackData->AbilityClass)
	{
		return;
	}

	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SpecialAttackData->AbilityClass);
	if (ActivatedAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UAttackStateBase:: ActivatedAbility is valid"));
		ActivatedAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedAbility->OnAbilityEnded.AddUObject(this, &UAttackStateBase::OnAttackAbilityEnded);
		LastUsedAttack = ActivatedAbility;
	}
	else
	{
		BroadcastTransition(GAS_Tags::TAG_AI_State_Movement, nullptr, "AttackAbility activation is failed.");
	}
}

void UAttackStateBase::OnComboEnd(const FEnemyComboChainEndData& EndData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "ComboChain is ended.");
}

void UAttackStateBase::OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "AttackAbility is ended.");
}

void UAttackStateBase::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (EnemyTagDelegatesComp) 
	{
		EnemyTagDelegatesComp->UnregisterAllDelegatesForObject(this);
	}

	if (IsValid(LastUsedAttack))
	{
		LastUsedAttack->OnAbilityEnded.RemoveAll(this);
		LastUsedAttack = nullptr;
	}
}