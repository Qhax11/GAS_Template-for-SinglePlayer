// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/AttackStateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
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
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();
	SelectAndMakeAttack();
}

bool UAttackStateBase::SelectAndMakeAttack()
{
	SelectedAttackClass = GetSelectedAttackAbilityData().AbilityClass;
	MakeAttack();
	return true;
}

void UAttackStateBase::MakeAttack()
{
	if (!SelectedAttackClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedAttackClass is null in: %s"), *GetName());
		return;
	}

	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);
	if (ActivatedAbility) 
	{
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

	if (LastUsedAttack)
	{
		LastUsedAttack->OnAbilityEnded.RemoveAll(this);
		LastUsedAttack = nullptr;
	}
}