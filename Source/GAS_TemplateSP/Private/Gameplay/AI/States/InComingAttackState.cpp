// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Abilities/InCombat/GA_TakeDamageBase.h"
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

bool UInComingAttackState::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!EnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UInComingAttackState: EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	InComingAttackStatePayload = StaticCastSharedPtr<FIncomingAttackStatePayload>(EnterPayload);
	if (!InComingAttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UInComingAttackState: InComingAttackStatePayload is invalid in: %s"), *GetName());
		return false;
	}

	if (!InComingAttackStatePayload->ReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInComingAttackState: ReactionData is null in: %s"), *GetName());
		return false;
	}

	if (!InComingAttackStatePayload->AttackPayload.ComingAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInComingAttackState: ComingAttack is null in: %s"), *GetName());
		return false;
	}

	return true;
}

void UInComingAttackState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	InComingAttackStatePayload = StaticCastSharedPtr<FIncomingAttackStatePayload>(EnterPayload);
	check(InComingAttackStatePayload.IsValid());
	check(InComingAttackStatePayload->ReactionData);
	check(InComingAttackStatePayload->AttackPayload.ComingAttack);

	// Always bind to DamageSubsystem here so that the state can respond to any incoming damage
    // regardless of the reaction type (take damage, parry, dodge). This ensures the state
    // can exit correctly if the AI takes damage during any reaction.  
	if (DamageSubsystem)
	{
		if (!DamageSubsystem->OnDamageDealt.IsAlreadyBound(this, &UInComingAttackState::OnDamageDealt))
		{
			DamageSubsystem->OnDamageDealt.AddDynamic(this, &UInComingAttackState::OnDamageDealt);
		}
		UE_LOG(LogTemp, Warning, TEXT("UInComingAttackState: DamageSubsystem binded."));
	}

	SelectAndExecuteReaction(InComingAttackStatePayload->ReactionData);
}

bool UInComingAttackState::SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData)
{
	if (!SelectedReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedReactionData is invalid in: %s"), *GetName());
		return false;
	}

	if (SelectedReactionData->ReactionExecutionMode == EReactionExecutionMode::Ability)
	{
		BindTargetComingAttackEnd();
		ExecuteAbilityReaction(SelectedReactionData);
		return true;
	}
	else if (SelectedReactionData->ReactionExecutionMode == EReactionExecutionMode::MovementAbility) 
	{

	}

	return false;
}

bool UInComingAttackState::ExecuteAbilityReaction(UComingAttackReactionData* ReactionData)
{
	return false;
}

bool UInComingAttackState::ExecuteMovementReaction(UComingAttackReactionData* SelectedReactionData)
{
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
	UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDamageDealt entered from: %s"), *GetClass()->GetName());
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
			UE_LOG(LogTemp, Warning, TEXT("State Manager: ParryKnocbackAbility executed from: %s"), *GetClass()->GetName());
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
	UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility entered."));

	if (LastUsedParryAbility && LastUsedParryAbility->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: LastUsedParryAbility is active from: %s"), *GetClass()->GetName());
		if (LastUsedParryAbility)
		{
			EnemyASC->CancelAbilityHandle(LastUsedParryAbility->GetCurrentAbilitySpecHandle());
			//LastUsedParryAbility->EndAbilityManually();
		}
	}

	const bool bIsInActiveAttackPhase = EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	if (bIsInActiveAttackPhase)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: bIsInActiveAttackPhase TRUE."));
	}

	FaceTargetBeforeParry();

	UGAS_GameplayAbilityBase* ActivatedParryAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(EnemyParryAbilityClass);
	if (ActivatedParryAbility && ActivatedParryAbility->IsActive())
	{
		if (ParryEndHandle.IsValid()) 
		{
			ActivatedParryAbility->OnAbilityEnded.RemoveAll(this);
		}
		ParryEndHandle = ActivatedParryAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State Manager: MakeParryAbility executed from: %s"), *GetClass()->GetName());
	}

	LastUsedParryAbility = ActivatedParryAbility;
}

void UInComingAttackState::FaceTargetBeforeParry()
{
	if (!Enemy || !HeroTarget)
	{
		return;
	}

	const FVector ToTarget = HeroTarget->GetActorLocation() - Enemy->GetActorLocation();
	FVector FlatDir = FVector(ToTarget.X, ToTarget.Y, 0.f);

	if (FlatDir.IsNearlyZero())
	{
		return;
	}

	const FRotator TargetRotation = FlatDir.Rotation();

	Enemy->SetActorRotation(TargetRotation);
}

void UInComingAttackState::OnParryAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback))
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: OnParryAbilityEnded with knocback don't exit the state from: %s"), *GetClass()->GetName());
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

	if (IsValid(LastUsedParryAbility) && ParryEndHandle.IsValid())
	{
		LastUsedParryAbility->OnAbilityEnded.Remove(ParryEndHandle);
		ParryEndHandle.Reset();
		LastUsedParryAbility = nullptr;
	}

	if (IsValid(LastUsedParryKnocbackAbility) && ParryKnockbackEndHandle.IsValid())
	{
		LastUsedParryKnocbackAbility->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
		LastUsedParryKnocbackAbility = nullptr;
	}
}

