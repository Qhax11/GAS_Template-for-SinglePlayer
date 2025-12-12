// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"


UAttackStateBase::UAttackStateBase()
{
	StateTag = GAS_Tags::TAG_AI_State_Attack;
}

void UAttackStateBase::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UAttackStateBase::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter();

	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	TSharedPtr<FAttackStatePayload> AttackStatePayload = StaticCastSharedPtr<FAttackStatePayload>(EnterPayload);
	if (!AttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackStatePayload is invalid in: %s"), *GetName());
		return;
	}

	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();
	SelectAndMakeAttack(AttackStatePayload);
}

bool UAttackStateBase::SelectAndMakeAttack(TSharedPtr<FAttackStatePayload> AttackStatePayload)
{
	MakeAttack(AttackStatePayload->AttackData.AbilityClass);
	return true;
}

void UAttackStateBase::MakeAttack(TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase:: MakeAttack is entered"));

	if (!SelectedAttackClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedAttackClass is null in: %s"), *GetName());
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