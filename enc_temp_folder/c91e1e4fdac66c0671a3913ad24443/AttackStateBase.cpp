// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"
#include "Gameplay/Utilities/Combat/CombatDistanceUtils.h"

UAttackStateBase::UAttackStateBase()
{
	StateTag = GAS_Tags::TAG_AI_State_Attack;
}

void UAttackStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

bool UAttackStateBase::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	TSharedPtr<FAttackStatePayload> AttackStatePayload = StaticCastSharedPtr<FAttackStatePayload>(EnterPayload);
	if (!AttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: AttackStatePayload is invalid in: %s"), *GetName());
		return false;
	}

	if (!AttackStatePayload->AttackData || !AttackStatePayload->AttackData->AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: AttackData or AbilityClass invalid in: %s"), *GetName());
		return false;
	}

	if (!IsValid(Enemy) || !IsValid(HeroTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: Enemy or HeroTarget is invalid in: %s"), *GetName());
		return false;
	}

	UGAS_GameplayAbilityBase* SelectedAttackCDO = AttackStatePayload->AttackData->AbilityClass->GetDefaultObject<UGAS_GameplayAbilityBase>();
	if (!SelectedAttackCDO) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: SelectedAttackCDO is null!"));
		return false;
	}

	float MaxRange = SelectedAttackCDO->MaxRange;
	if (!CombatDistance::IsInRange(Enemy, HeroTarget, MaxRange))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: Out of Range!"));
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
	check(AttackStatePayload->AttackData);
	check(AttackStatePayload->AttackData->AbilityClass);

	ExecuteAttack(AttackStatePayload->AttackData->AbilityClass);
}

void UAttackStateBase::ExecuteAttack(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass)
{
	if (!SelectedAttackClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: SelectedAttackClass is null in: %s"), *GetName());
		return;
	}

	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);
	if (ActivatedAbility) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase:: ActivatedAbility is valid"));
		ActivatedAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedAbility->OnAbilityEnded.AddUObject(this, &UAttackStateBase::OnAttackAbilityEnded);
		LastUsedAttack = ActivatedAbility;
	}
	else
	{
		ExitRequest("Attack activation failed");
	}
}

void UAttackStateBase::OnAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	ExitRequest("OnAttackAbilityEnded");
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