// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Parry.h"
#include "Gameplay/Abilities/InCombat/GA_TakeDamageBase.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"
#include <Gameplay/StaticDelegates/S_DamageDelegates.h>
#include "Gameplay/AI/Components/AC_StateManager.h"


UInComingAttackState::UInComingAttackState()
{
	StateTag = GAS_Tags::TAG_AI_State_InComingAttack;
}

void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
	DamageSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_DamageDelegates>();
}

bool UInComingAttackState::EnterCondition_Implementation()
{
	FComingAttackPayload ComingAttackPayload = StateManager->ComingAttackPayload;
	float ComingAttackMaxRange = ComingAttackPayload.ComingAttack->MaxRange;

	const float Distance = HeroTarget->GetDistanceTo(Enemy);
	const bool bIsInRange = Distance < ComingAttackMaxRange;

	const bool bEnemyUnstoppable = EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);
	const bool bHeroCanInterrupt = HeroTargetASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanInterruptUnstoppableAttack);

	// Mantık:
	// 1. Eğer düşman unstoppable ise ama kahraman interrupt edebiliyorsa => izin ver
	// 2. Eğer düşman unstoppable ve kahraman edemiyorsa => girme
	// 3. Eğer düşman unstoppable değilse => normal şekilde distance'a göre değerlendir
	if (bEnemyUnstoppable && !bHeroCanInterrupt)
	{
		return false;
	}

	return bIsInRange;
}

void UInComingAttackState::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	// Always bind to DamageSubsystem here so that the state can respond to any incoming damage
	// regardless of the reaction type (take damage, parry, dodge). This ensures the state
	// can exit correctly if the AI takes damage during any reaction.  
	if (DamageSubsystem)
	{
		if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UInComingAttackState::OnDamageDealt))
		{
			DamageSubsystem->OnDamageDealt.AddDynamic(this, &UInComingAttackState::OnDamageDealt);
		}
		UE_LOG(LogTemp, Warning, TEXT("State Manager: DamageSubsystem binded."));
	}

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
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeTakeDamage entered."));
		BindTargetComingAttackEnd();
		return true;
	}
	else if (SelectedBestReaction->ReactionType == EComingAttackReaction::Parry)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility entered."));
		BindTargetComingAttackEnd();
		MakeParryAbility(SelectedBestReaction);
		return true;
	}
	return false;
}

void UInComingAttackState::BindTargetComingAttackEnd()
{
	UGAS_GameplayAbilityBase* ComingAttack = StateManager->ComingAttackPayload.ComingAttack;
	if (ComingAttack)
	{
		ComingAttack->OnAbilityEnded.RemoveAll(this);
		ComingAttack->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnComingAttackAbilityEnded);
	}
	LastComingAttackAbility = ComingAttack;
}

void UInComingAttackState::UnBindTargetComingAttackEnd()
{
	if (IsValid(LastComingAttackAbility))
	{
		LastComingAttackAbility->OnAbilityEnded.RemoveAll(this);
		LastComingAttackAbility = nullptr;
	}
}

void UInComingAttackState::OnDamageDealt(const FDamageData& DamageData)
{
	UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDamageDealt entered."));

	if (DamageData.ExecCalculationParameters.TargetActor != Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDamageDealt TargetActor is not Enemy."));
		return;
	}

	UnBindTargetComingAttackEnd();

	if (DamageData.bParrySucces)
	{
		FGameplayEventData Payload;
		Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;
		Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
		Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
		Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
		Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

		UGAS_GameplayAbilityBase* ParryKnocbackAbility = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyParryKnocbackAbilityClass, Payload);
		if (ParryKnocbackAbility && ParryKnocbackAbility->IsActive())
		{
			ParryKnocbackAbility->OnAbilityEnded.RemoveAll(this);
			ParryKnocbackAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryKnocbackAbilityEnded);
			UE_LOG(LogTemp, Warning, TEXT("State Manager: ParryKnocbackAbility executed."));
		}
		LastUsedParryKnocbackAbility = ParryKnocbackAbility;
	}
	else
	{
		// Prepare payload
		FGameplayEventData Payload;
		Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_TakeDamage;
		Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
		Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
		Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
		Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

		UGAS_GameplayAbilityBase* TakeDamageAbility = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyTakeDamageAbilityClass, Payload);
		if (TakeDamageAbility && TakeDamageAbility->IsActive())
		{
			TakeDamageAbility->OnAbilityEnded.RemoveAll(this);
			TakeDamageAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnTakeDamageAbilityEnded);
			UE_LOG(LogTemp, Warning, TEXT("State Manager: TakeDamageAbility executed."));
		}
		LastUsedTakeDamageAbility = TakeDamageAbility;
	}
}

void UInComingAttackState::OnTakeDamageAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	ExitRequest("OnTakeDamageAbilityEnded");
}

void UInComingAttackState::OnComingAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	ExitRequest("OnComingAttackAbilityEnded");
}

void UInComingAttackState::MakeParryAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	//Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	//Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

	if (LastUsedParryAbility && LastUsedParryAbility->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: LastUsedParryAbility is active."));
		if (LastUsedParryAbility)
		{
			EnemyASC->CancelAbilityHandle(LastUsedParryAbility->GetCurrentAbilitySpecHandle());
			//LastUsedParryAbility->EndAbilityManually();
		}
	}

	UGAS_GameplayAbilityBase* ActivatedParryAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(EnemyParryAbilityClass);
	if (ActivatedParryAbility && ActivatedParryAbility->IsActive())
	{
		ActivatedParryAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedParryAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility executed."));
	}

	LastUsedParryAbility = ActivatedParryAbility;
}

void UInComingAttackState::OnParryAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback))
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: OnParryAbilityEnded with knocback don't exit the state"));
		return;
	}
	else
	{
		ExitRequest("Parry Ability Ended without knocback.");
	}
}

void UInComingAttackState::OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	ExitRequest("OnParryKnocbackAbilityEnded");
}

void UInComingAttackState::OnExit_Implementation()
{
	if (!IsInGameThread())
	{
		AsyncTask(ENamedThreads::GameThread, [WeakThis = TWeakObjectPtr<UInComingAttackState>(this)]()
			{
				if (UInComingAttackState* Self = WeakThis.Get())
				{
					Self->OnExit_Implementation();
				}
			});
		return;
	}

	Super::OnExit_Implementation();

	CleanupDelegates();
	UnBindTargetComingAttackEnd();

	/*
	// DEFER delegate cleanup - broadcast bitene kadar bekle
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr<UInComingAttackState>(this)]()
			{
				if (UInComingAttackState* Self = WeakThis.Get())
				{
					Self->CleanupDelegates();
					Self->UnBindTargetComingAttackEnd();
				}
			});
	}
	else
	{
		// World yoksa direkt yap (BeginDestroy vs)
		CleanupDelegates();
	}
	*/
}

void UInComingAttackState::CleanupDelegates()
{
	if (IsValid(DamageSubsystem))
	{
		DamageSubsystem->OnDamageDealt.RemoveAll(this);
	}

	if (IsValid(Enemy) && Enemy->GetTagDelegatesComponent())
	{
		Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);
	}

	if (IsValid(LastUsedTakeDamageAbility))
	{
		LastUsedTakeDamageAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedTakeDamageAbility = nullptr;
	}

	if (IsValid(LastUsedParryAbility))
	{
		LastUsedParryAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedParryAbility = nullptr;
	}

	if (IsValid(LastUsedParryKnocbackAbility))
	{
		LastUsedParryKnocbackAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedParryKnocbackAbility = nullptr;
	}
}

