// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Parry.h"
#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"
#include <Gameplay/StaticDelegates/S_DamageDelegates.h>


UInComingAttackState::UInComingAttackState()
{
	StateTag = GAS_Tags::TAG_AI_State_InComingAttack;
}

void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
	DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>();
}

void UInComingAttackState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();



	SelectAndMakeInComingAttackReaction();
}

bool UInComingAttackState::SelectAndMakeInComingAttackReaction()
{
	UBDS_ComingAttackReactionBase* SelectedBestReaction = BehaviorDecisionComponent->LastSelectedComingAttackReaction;
	if (!SelectedBestReaction) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedBestReaction is null in: %s"), *GetName());
		return false;
	}

	if (SelectedBestReaction->ReactionType == EComingAttackReaction::TakeDamage)
	{
		MakeTakeDamage(SelectedBestReaction);
		return true;
	}
	else if(SelectedBestReaction->ReactionType == EComingAttackReaction::Parry)
	{
		MakeParryAbility(SelectedBestReaction);
		return true;
	}

	return false;
}

void UInComingAttackState::MakeTakeDamage(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	if (DamageSubsystem)
	{
		if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UInComingAttackState::OnDamageDealt))
		{
			DamageSubsystem->OnDamageDealt.AddDynamic(this, &UInComingAttackState::OnDamageDealt);
		}
	}

	UGAS_GameplayAbilityBase* ComingAttack = StateManager->ComingAttackPayload.ComingAttack;
	if (ComingAttack)
	{
		if (!ComingAttack->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnComingAttackAbilityEnded))
		{
			ComingAttack->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnComingAttackAbilityEnded);
		}
	}

	LastComingAttackAbility = ComingAttack;
}

void UInComingAttackState::OnDamageDealt(const FDamageData& DamageData)
{
	if (DamageData.ExecCalculationParameters.TargetActor != Enemy) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDamageDealt TargetActor is Enemy."));
		return;
	}

	// Prepare payload
	FGameplayEventData Payload;
	Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_TakeDamage;
	Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
	Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
	Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
	Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

	UGAS_GameplayAbilityBase* TakeDamageAbility = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyTakeDamageAbilityClass, Payload);
	if (TakeDamageAbility)
	{
		if (!TakeDamageAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnTakeDamageAbilityEnded))
		{
			TakeDamageAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnTakeDamageAbilityEnded);
		}
	}

	LastUsedTakeDamageAbility = TakeDamageAbility;

	if (LastComingAttackAbility)
	{
		if (LastComingAttackAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnComingAttackAbilityEnded))
		{
			LastComingAttackAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnComingAttackAbilityEnded);
		}
		LastComingAttackAbility = nullptr;
	}
}

void UInComingAttackState::OnTakeDamageAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest("OnTakeDamageAbilityEnded");
}

void UInComingAttackState::OnComingAttackAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest("OnComingAttackAbilityEnded");
}

void UInComingAttackState::MakeParryAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility function entered."));

	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

	if (LastUsedParryAbility) 
	{
		if (LastUsedParryAbility->IsActive())
		{
			UE_LOG(LogTemp, Warning, TEXT("State Manager: LastUsedParryAbility is active."));
			if (LastUsedParryAbility)
			{
				LastUsedParryAbility->EndAbilityManually();
			}
		}
	}

	UGAS_GameplayAbilityBase* ActivatedParryAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(EnemyParryAbilityClass);
	if (ActivatedParryAbility)
	{
		if (!ActivatedParryAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnParryAbilityEnded))
		{
			ActivatedParryAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnParryAbilityEnded);
		}
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility executed."));
	}

	LastUsedParryAbility = ActivatedParryAbility;

}

void UInComingAttackState::OnParryAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback))
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: OnParryAbilityEnded with knocback, now we listen knocback removed for exit"));
		EnemyTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagRemoved);
	}
	else
	{
		ExitRequest("Parry Ability Ended without knocback.");
	}
}

void UInComingAttackState::OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	ExitRequest("OnParryKnocbackTagRemoved");
}

void UInComingAttackState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (IsValid(Enemy) && Enemy->GetTagDelegatesComponent())
	{
		Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);
	}

	if (DamageSubsystem)
	{
		if (DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UInComingAttackState::OnDamageDealt))
		{
			DamageSubsystem->OnDamageDealt.RemoveDynamic(this, &UInComingAttackState::OnDamageDealt);
		}
	}

	if (LastUsedTakeDamageAbility)
	{
		if (LastUsedTakeDamageAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnTakeDamageAbilityEnded))
		{
			LastUsedTakeDamageAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnTakeDamageAbilityEnded);
			UE_LOG(LogTemp, Warning, TEXT("State Manager: %s ability's end bind is removed."), *LastUsedTakeDamageAbility->GetName());
		}
	}

	if (LastUsedParryAbility)
	{
		if (LastUsedParryAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnParryAbilityEnded))
		{
			LastUsedParryAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnParryAbilityEnded);
			UE_LOG(LogTemp, Warning, TEXT("State Manager: %s ability's end bind is removed."), *LastUsedParryAbility->GetName());
		}
	}
}


