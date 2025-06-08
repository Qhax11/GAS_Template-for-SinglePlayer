// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"


void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
	EnemyASC->AbilityActivatedCallbacks.AddUObject(this, &UInComingAttackState::OnTargetAbilityActivated);
}

void UInComingAttackState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	SelectAndMakeInComingAttackReaction();
}

void UInComingAttackState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this); // test?

	if (LastUsedDodgeAbility)
	{
		if (LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnDodgeAbilityEnded))
		{
			LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnDodgeAbilityEnded);
		}
		LastUsedDodgeAbility = nullptr;
	}

	if (LastUsedTakeDamageAbility)
	{
		if (LastUsedTakeDamageAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnTakeDamageAbilityEnded))
		{
			UE_LOG(LogTemp, Warning, TEXT("LastUsedTakeDamageAbility, REMOVED BIND!"));
			LastUsedTakeDamageAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UInComingAttackState::OnTakeDamageAbilityEnded);
		}
		LastUsedTakeDamageAbility = nullptr;
	}

}

void UInComingAttackState::SelectAndMakeInComingAttackReaction()
{
	FComingAttackReactionData SelectedReactionData = GetSelectedReactionData();

	switch (SelectedReactionData.ReactionType)
	{
	case EComingAttackReaction::Parry:
		MakeParryAbility(SelectedReactionData);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeParryAbility"));
		break;

	case EComingAttackReaction::Dodge:
		ActivateDodgeAbility(SelectedReactionData);
		UE_LOG(LogTemp, Warning, TEXT("Triggered ActivateDodgeAbility"));
		break;

	case EComingAttackReaction::TakeDamage:
		MakeTakeDamage(SelectedReactionData);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeTakeDamage"));
		break;
	}
}

void UInComingAttackState::TriggerIncomingReaction(FComingAttackReactionData Reaction)
{
	const FComingAttackPayload& Payload = StateManager->ComingAttackPayload;
	const FComingAttackReactionData BestReaction = BehaviorDecisionComponent->GetBestComingAttackDecision(Payload);

	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

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
		MakeTakeDamage(Reaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeTakeDamage"));
		break;
	}
}

void UInComingAttackState::MakeTakeDamage(FComingAttackReactionData BestComingAttackReaction)
{
	// Komboyu/movement'i durdurma buraya da ekleyebilirsin ama zaten TriggerIncomingReaction içinde var.

// Eğer ability aktif edilirse dinlenecek zaten
// Şimdi failsafe başlat — eğer ability 0.2 saniye içinde aktive edilmezse çık

	Enemy->GetWorldTimerManager().SetTimer(TakeDamageFailsafeTimer, this,
		&UInComingAttackState::OnTakeDamageFailsafeTimeout,
		0.2f, false);

	UE_LOG(LogTemp, Warning, TEXT("TakeDamage failsafe timer started."));
}

void UInComingAttackState::OnTakeDamageFailsafeTimeout()
{
	UE_LOG(LogTemp, Warning, TEXT("Failsafe triggered: TakeDamage ability not activated in time."));
	ExitRequest();
}

void UInComingAttackState::OnTargetAbilityActivated(UGameplayAbility* Ability)
{
	UGA_TakeDamageBase* TakeDamageAbility = Cast<UGA_TakeDamageBase>(Ability);
	if (!TakeDamageAbility)
	{
		return;
	}

	// Timer’ı durdur, çünkü ability gerçekten aktive oldu
	Enemy->GetWorldTimerManager().ClearTimer(TakeDamageFailsafeTimer);

	if (!TakeDamageAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UInComingAttackState::OnTakeDamageAbilityEnded))
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamageAbility Binded!"));
		TakeDamageAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UInComingAttackState::OnTakeDamageAbilityEnded);
	}
	LastUsedTakeDamageAbility = TakeDamageAbility;
}

void UInComingAttackState::OnTakeDamageAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTakeDamageAbilityEnded!"));
	ExitRequest();
}

void UInComingAttackState::MakeParryAbility(FComingAttackReactionData BestComingAttackReaction)
{
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
	UE_LOG(LogTemp, Warning, TEXT("OnDodgeAbilityEnded, exiting."));
	ExitRequest();
}



