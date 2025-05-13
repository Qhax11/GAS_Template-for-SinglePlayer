// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"

void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UInComingAttackState::OnEnter()
{
	Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);

	FComingAttackReactionData BestComingAttackReaction = BehaviorDecisionComponent->GetBestComingAttackDecision(StateManager->ComingAttackPayload);
	switch (BestComingAttackReaction.ReactionType)
	{
	case EComingAttackReaction::Parry:
		Enemy->GetTagDelegatesComponent()->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagRemoved);
		ActivateParryAbility(BestComingAttackReaction);
		break;

	case EComingAttackReaction::Dodge:
		ActivateDodgeAbility(BestComingAttackReaction);
		break;

	case EComingAttackReaction::TakeDamage:
		Enemy->GetTagDelegatesComponent()->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnTakeDamageTagRemoved);
		break;
	}
}

void UInComingAttackState::OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	ExitRequest();
}

void UInComingAttackState::ActivateParryAbility(FComingAttackReactionData BestComingAttackReaction)
{
	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassAndReturnInstance(BestComingAttackReaction.RecationAbilityClass);
	UE_LOG(LogTemp, Warning, TEXT("ActivateParryAbility"));
	/*
	if (ActivatedAbility)
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnParryAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnParryAbilityEnded);
		}
	}
	*/
}

void UInComingAttackState::OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("OnParryKnocbackTagRemoved"));
	ExitRequest();
}

void UInComingAttackState::ActivateDodgeAbility(FComingAttackReactionData BestComingAttackReaction)
{
	FGameplayEventData GameplayEventData = FGameplayEventData();
	GameplayEventData.InstigatorTags.AddTag(GAS_Tags::TAG_AI_Direction_Resolved_Backward);
	GameplayEventData.EventTag = GAS_Tags::TAG_AI_AbilityTriggerEvent_Movement_Dash;

	UGAS_GameplayAbilityBase* ActivatedAbility =
	EnemyASC->TryActivateAbilityByClassWithEventData(BestComingAttackReaction.RecationAbilityClass, GameplayEventData);
	if (ActivatedAbility)
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnDodgeAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnDodgeAbilityEnded);
		}
	}
}
void UInComingAttackState::OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest();
}



