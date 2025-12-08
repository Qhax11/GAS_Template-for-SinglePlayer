// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
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

bool UInComingAttackState::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!EnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	InComingAttackStatePayload = StaticCastSharedPtr<FIncomingAttackStatePayload>(EnterPayload);
	if (!InComingAttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("InComingAttackStatePayload is invalid in: %s"), *GetName());
		return false;
	}

	FComingAttackPayload ComingAttackPayload = InComingAttackStatePayload->AttackPayload;
	float ComingAttackMaxRange = ComingAttackPayload.ComingAttack->MaxRange;

	const float Distance = HeroTarget->GetDistanceTo(Enemy);
	const bool bIsInRange = Distance < ComingAttackMaxRange + 50.0f; // 50.0f is margin of error

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

void UInComingAttackState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	if (!EnterPayload.IsValid() || !InComingAttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EnterPayload is invalid in: %s"), *GetName());
		return;
	}

	SelectAndExecuteReaction(InComingAttackStatePayload->ReactionData);

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
}

bool UInComingAttackState::SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData)
{
	if (!SelectedReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedReactionData is invalid in: %s"), *GetName());
		return false;
	}

	if (SelectedReactionData->ReactionType == EComingAttackReaction::TakeDamage)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeTakeDamage entered."));
		BindTargetComingAttackEnd();
		return true;
	}
	else if (SelectedReactionData->ReactionType == EComingAttackReaction::Parry)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility entered."));
		BindTargetComingAttackEnd();
		MakeParryAbility(SelectedReactionData);
		return true;
	}

	return false;
}

void UInComingAttackState::BindTargetComingAttackEnd()
{
	UGAS_GameplayAbilityBase* ComingAttack = InComingAttackStatePayload->AttackPayload.ComingAttack;
	if (ComingAttack)
	{
		if (ComingAttackEndHandle.IsValid()) 
		{
			ComingAttack->OnAbilityEnded.RemoveAll(this);
		}
		ComingAttackEndHandle = ComingAttack->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnComingAttackAbilityEnded);
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

	if (DamageData.ExecCalculationParameters.SourceActor == Enemy && DamageData.ExecCalculationParameters.TargetActor == HeroTarget)
	{
		ExitRequest("Our attack is hit to Hero, exit incoming attack.");
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
			if (ParryKnockbackEndHandle.IsValid()) 
			{
				ParryKnocbackAbility->OnAbilityEnded.RemoveAll(this);
			}
			ParryKnockbackEndHandle = ParryKnocbackAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryKnocbackAbilityEnded);
			UE_LOG(LogTemp, Warning, TEXT("State Manager: ParryKnocbackAbility executed."));
		}
		LastUsedParryKnocbackAbility = ParryKnocbackAbility;
	}

}

void UInComingAttackState::OnComingAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	ExitRequest("OnComingAttackAbilityEnded");
}

void UInComingAttackState::MakeParryAbility(const UComingAttackReactionData* BestComingAttackReaction)
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
		if (ParryEndHandle.IsValid()) 
		{
			ActivatedParryAbility->OnAbilityEnded.RemoveAll(this);
		}
		ParryEndHandle = ActivatedParryAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryAbilityEnded);
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

	// Hemen cleanup yap, sorun yok artık!
	CleanupDelegates();
	UnBindTargetComingAttackEnd();
}

void UInComingAttackState::CleanupDelegates()
{
	// Bu fonksiyon defer edildiği için güvenli şekilde RemoveAll kullanabilir
	if (IsValid(DamageSubsystem))
	{
		DamageSubsystem->OnDamageDealt.RemoveAll(this);
	}

	if (IsValid(Enemy) && Enemy->GetTagDelegatesComponent())
	{
		Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);
	}

	// Parry
	if (IsValid(LastUsedParryAbility) && ParryEndHandle.IsValid())
	{
		LastUsedParryAbility->OnAbilityEnded.Remove(ParryEndHandle);
		ParryEndHandle.Reset();
		LastUsedParryAbility = nullptr;
	}

	// ParryKnockback
	if (IsValid(LastUsedParryKnocbackAbility) && ParryKnockbackEndHandle.IsValid())
	{
		LastUsedParryKnocbackAbility->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
		LastUsedParryKnocbackAbility = nullptr;
	}
}

