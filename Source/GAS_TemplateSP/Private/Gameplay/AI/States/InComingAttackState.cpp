// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"

void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UInComingAttackState::OnEnter_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("InComingAttack has been enter"));

	bStateFinished = false;
	SelectAndMakeInComingAttackReaction();
}

void UInComingAttackState::OnExit_Implementation()
{
	Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this); // test?

	if (LastUsedDodgeAbility)
	{
		if (LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnDodgeAbilityEnded))
		{
			LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnDodgeAbilityEnded);
		}
		LastUsedDodgeAbility = nullptr;
	}
}

void UInComingAttackState::SelectAndMakeInComingAttackReaction()
{
	const FComingAttackPayload& Payload = StateManager->ComingAttackPayload;
	const FComingAttackReactionData BestReaction = BehaviorDecisionComponent->GetBestComingAttackDecision(Payload);

	const float TimeToHit = Payload.ComingAttackHitTime;
	const float PreferredDelay = TimeToHit - BestReaction.PreferredTriggerTimeBeforeHit;

	// Çok geç kaldıysak, hemen uygula
	if (PreferredDelay <= 0.f)
	{
		TriggerIncomingReaction(BestReaction);
	}
	else
	{
		// Timer kur, ideal zamanda reaction tetiklenecek
		Enemy->GetWorldTimerManager().SetTimer(DelayedReactionTimerHandle, FTimerDelegate::CreateUObject(
			this, &UInComingAttackState::TriggerIncomingReaction, BestReaction), PreferredDelay, false);

		UE_LOG(LogTemp, Warning, TEXT("Delaying Reaction by %.2f seconds..."), PreferredDelay);
	}
}

void UInComingAttackState::TriggerIncomingReaction(FComingAttackReactionData Reaction)
{
	switch (Reaction.ReactionType)
	{
	case EComingAttackReaction::Parry:
		MakeParryAbility(Reaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeParryAbility"));
		break;

	case EComingAttackReaction::Dodge:
		ActivateDodgeAbility(Reaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered ActivateDodgeAbility"));
		break;

	case EComingAttackReaction::TakeDamage:
		MakeTakeDamage();
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeTakeDamage"));
		break;
	}
}

void UInComingAttackState::MakeTakeDamage()
{
	// Failsafe: Eğer tag hiç eklenmezse ya da hiç çıkarılmazsa bu süre sonra exit
	Enemy->GetWorldTimerManager().SetTimer(TakeDamageFailsafeTimer, this,
		&UInComingAttackState::OnTakeDamageFailsafeTimeout,
		1.0f, false); // 1 saniye sonra çık (ayarlanabilir)

	Enemy->GetTagDelegatesComponent()->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnTakeDamageTagRemoved);
}

void UInComingAttackState::OnTakeDamageFailsafeTimeout()
{
	Enemy->GetWorldTimerManager().ClearTimer(TakeDamageFailsafeTimer);
	UE_LOG(LogTemp, Warning, TEXT("Failsafe: TakeDamage tag did not trigger, exiting InComingAttackState."));
	ExitRequest();
}

void UInComingAttackState::OnTakeDamageTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	Enemy->GetWorldTimerManager().ClearTimer(TakeDamageFailsafeTimer);
	ExitRequest();
}

void UInComingAttackState::MakeParryAbility(FComingAttackReactionData BestComingAttackReaction)
{
	// Stop any current combo before parrying
	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

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
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

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

		LastUsedDodgeAbility = ActivatedAbility;
	}
}
void UInComingAttackState::OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest();
}



