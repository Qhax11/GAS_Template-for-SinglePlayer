// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"

void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UInComingAttackState::OnEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEnter to InComingAttackState"));

	Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);

	FComingAttackReactionData BestComingAttackReaction = BehaviorDecisionComponent->GetBestComingAttackDecision(StateManager->ComingAttackPayload);
	switch (BestComingAttackReaction.ReactionType)
	{
	case EComingAttackReaction::Parry:
		MakeParryAbility(BestComingAttackReaction);
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

void UInComingAttackState::MakeParryAbility(FComingAttackReactionData BestComingAttackReaction)
{
	// Stop any current combo before parrying
	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();

	// Activate the parry ability
	EnemyASC->TryActivateAbilityByClassAndReturnInstance(BestComingAttackReaction.RecationAbilityClass);

	// Clear state flags before binding
	bParryKnockbackHappened = false;

	// Register tag listeners
	UAC_TagDelegates* TagDelegates = Enemy->GetTagDelegatesComponent();
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryTagRemoved);
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagAdded);
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagRemoved);

	UE_LOG(LogTemp, Warning, TEXT("MakeParryAbility executed."));
}

void UInComingAttackState::OnParryTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (bParryKnockbackHappened)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parry ended but knockback already triggered. Ignoring."));
		return;
	}

	// Delay-exit if knockback never happens
	FTimerHandle DelayHandle;
	Enemy->GetWorldTimerManager().SetTimer(DelayHandle, [this]()
		{
			if (!bParryKnockbackHappened)
			{
				UE_LOG(LogTemp, Warning, TEXT("Parry ended, no knockback happened. Exiting."));
				ExitRequest();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Parry ended, but knockback happened after. Exit handled elsewhere."));
			}
		}, 0.1f, false); // 0.05–0.1s delay is usually safe for reaction window
}

void UInComingAttackState::OnParryKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	bParryKnockbackHappened = true;
	UE_LOG(LogTemp, Warning, TEXT("ParryKnockback tag added."));
}

void UInComingAttackState::OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("ParryKnockback tag removed. Exiting."));
	ExitRequest();
}

void UInComingAttackState::ActivateDodgeAbility(FComingAttackReactionData BestComingAttackReaction)
{
	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();

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



