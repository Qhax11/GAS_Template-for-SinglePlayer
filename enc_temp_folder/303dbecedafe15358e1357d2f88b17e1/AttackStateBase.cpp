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

	bPhaseTagCleared = false;
	bAbilityEnded = false;

	EnemyTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_PostAttack, EListenMode::OnRemoved).
		BindDynamic(this, &UAttackStateBase::OnActivePhasePostHitTagRemoved);

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

	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_TakeHit))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase: TAG_Gameplay_State_Phase_Active_TakeHit is still exist!"));
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
	UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase:: OnAttackAbilityEnded entered."))
	bAbilityEnded = true;
	TryExitState();
}

void UAttackStateBase::OnActivePhasePostHitTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("UAttackStateBase:: OnActivePhasePostHitTagRemoved entered."))
	bPhaseTagCleared = true;
	TryExitState();
}

void UAttackStateBase::TryExitState()
{
	if (bAbilityEnded && bPhaseTagCleared)
	{
		ExitRequest("TakeHitFinished");
	}
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