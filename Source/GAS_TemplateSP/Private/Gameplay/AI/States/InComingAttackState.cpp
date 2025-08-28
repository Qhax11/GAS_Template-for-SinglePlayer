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
		}
		LastUsedTakeDamageAbility = nullptr;
	}

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

	UGAS_GameplayAbilityBase* TakeDamageAbility = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyTakeDamage, Payload);
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
	BDS_Parry = Cast<UBDS_ComingAttackReaction_Parry>(BestComingAttackReaction);
	if (!BDS_Parry)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: BDS_Parry is null in: %s"), *GetName());
		return;
	}

	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

	// Activate the parry ability
	EnemyASC->TryActivateAbilityByClassAndReturnInstance(BDS_Parry->ParryAbilityClass);

	// Clear state flags before binding
	bParryKnockbackHappened = false;

	// Register tag listeners
	UAC_TagDelegates* TagDelegates = Enemy->GetTagDelegatesComponent();
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_Parry, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryTagRemoved);
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagAdded);
	TagDelegates->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnRemoved).BindDynamic(this, &UInComingAttackState::OnParryKnocbackTagRemoved);

	UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility executed."));
}

void UInComingAttackState::OnParryTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	if (bParryKnockbackHappened)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: Parry ended but knockback already triggered. Ignoring."));
		return;
	}

	// Delay-exit if knockback never happens
	FTimerHandle DelayHandle;
	Enemy->GetWorldTimerManager().SetTimer(DelayHandle, [this]()
		{
			if (!bParryKnockbackHappened)
			{
				UE_LOG(LogTemp, Warning, TEXT("Parry ended, no knockback happened. Exiting."));
				ExitRequest("bParryKnockbackHappened");
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("State Manager: Parry ended, but knockback happened after. Exit handled elsewhere."));
			}
		}, 0.1f, false); // 0.05–0.1s delay is usually safe for reaction window
}

void UInComingAttackState::OnParryKnocbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	bParryKnockbackHappened = true;

	if (BDS_Parry->bCounterImmediatelyAfterParry)
	{
		FTimerHandle DelayHandle;
		Enemy->GetWorldTimerManager().SetTimer(DelayHandle, [this]()
			{
				ExitRequest("State Manager: ParryKnockbackTimer");
			}, 0.1f, false); 
	}
}

void UInComingAttackState::OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	ExitRequest("OnParryKnocbackTagRemoved");
}



