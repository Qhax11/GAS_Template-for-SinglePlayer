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
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	// StaticCastSharedPtr is fast and safe if we trust the logic flow.
	InComingAttackStatePayload = StaticCastSharedPtr<FIncomingAttackStatePayload>(EnterPayload);
	if (!InComingAttackStatePayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: InComingAttackStatePayload is invalid in: %s"), *GetName());
		return false;
	}

	if (!InComingAttackStatePayload->ReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ReactionData is null in: %s"), *GetName());
		return false;
	}

	if (!InComingAttackStatePayload->AttackPayload.ComingAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ComingAttack is null in: %s"), *GetName());
		return false;
	}

	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack))
	{
		UE_LOG(LogTemp, Log, TEXT("State: UTakeHitState: Enemy is on Active Attack, not allowed enter the InComingAttackState"), *GetName());
		return false;
	}

	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack))
	{
		UE_LOG(LogTemp, Log, TEXT("State: UTakeHitState: Enemy is on Unstoppable Attack, not allowed enter the InComingAttackState"), *GetName());
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
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: DamageSubsystem binded."));
	}

	SelectAndExecuteReaction(InComingAttackStatePayload->ReactionData);
}

bool UInComingAttackState::SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData)
{
	if (!SelectedReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: SelectedReactionData is invalid in: %s"), *GetName());
		return false;
	}

	if (SelectedReactionData->ReactionType == EReactionType::Parry)
	{
		BindTargetComingAttackEnd();
		ExecuteParry(SelectedReactionData);
		return true;
	}

	return false;
}

void UInComingAttackState::BindTargetComingAttackEnd()
{
	UGAS_GameplayAbilityBase* ComingAttack = InComingAttackStatePayload->AttackPayload.ComingAttack;
	if (!IsValid(ComingAttack))
	{
		return;
	}

	// Remove previous binding from previous ability instance
	if (ComingAttackEndHandle.IsValid() && IsValid(LastComingAttack))
	{
		LastComingAttack->OnAbilityEnded.Remove(ComingAttackEndHandle);
		ComingAttackEndHandle.Reset();
	}

	ComingAttackEndHandle = ComingAttack->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnComingAttackAbilityEnded);
	LastComingAttack = ComingAttack;
}

void UInComingAttackState::OnComingAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "ComingAttackAbility is ended.");
}

void UInComingAttackState::OnDamageDealt(const FDamageData& DamageData)
{
	if (DamageData.ExecCalculationParameters.TargetActor != Enemy)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: OnDamageDealt entered from: %s"), *GetClass()->GetName());
	UnBindTargetComingAttackEnd();

	if (DamageData.bParrySucces)
	{
		ExecuteParryKnocback(DamageData);
	}
	else
	{
		BroadcastTransition(FGameplayTag(), nullptr, "Parry failed (took damage).");
	}
}

void UInComingAttackState::UnBindTargetComingAttackEnd()
{
	if (ComingAttackEndHandle.IsValid() && IsValid(LastComingAttack)) 
	{
		LastComingAttack->OnAbilityEnded.Remove(ComingAttackEndHandle); 
		ComingAttackEndHandle.Reset();
	}

	LastComingAttack = nullptr;
}

void UInComingAttackState::ExecuteParry(const UComingAttackReactionData* BestComingAttackReaction)
{
	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: MakeParryAbility entered."));

	if (IsValid(LastUsedParry) && LastUsedParry->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: LastUsedParryAbility is active from: %s"), *GetClass()->GetName());
		LastUsedParry->EndAbilityManually();
	}

	FaceTargetBeforeParry();

	UGAS_GameplayAbilityBase* ActivatedParryAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(EnemyParryAbilityClass);
	if (IsValid(ActivatedParryAbility) && ActivatedParryAbility->IsActive())
	{
		// Remove previous binding safely
		if (ParryEndHandle.IsValid() && IsValid(LastUsedParry))
		{
			LastUsedParry->OnAbilityEnded.Remove(ParryEndHandle);
			ParryEndHandle.Reset();
		}

		ParryEndHandle = ActivatedParryAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: MakeParryAbility executed from: %s"), *GetClass()->GetName());
	}

	LastUsedParry = ActivatedParryAbility;
}

void UInComingAttackState::ExecuteParryKnocback(const FDamageData& DamageData)
{
	FGameplayEventData Payload;
	Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;
	Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
	Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
	Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
	Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

	UGAS_GameplayAbilityBase* Knockback = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyParryKnocbackAbilityClass, Payload);

	// ❗If activation failed, don't wait for end delegates that will never fire.
	if (!IsValid(Knockback) || !Knockback->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ParryKnocback activation FAILED -> exiting"));
		BroadcastTransition(FGameplayTag(), nullptr, "ParryKnockback activation failed.");
		LastUsedParryKnocback = Knockback;
		return;
	}

	// Remove previous binding safely
	if (ParryKnockbackEndHandle.IsValid() && IsValid(LastUsedParryKnocback))
	{
		LastUsedParryKnocback->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
	}

	ParryKnockbackEndHandle = Knockback->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryKnocbackAbilityEnded);
	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ParryKnocbackAbility executed from: %s"), *GetClass()->GetName());

	LastUsedParryKnocback = Knockback;
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
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ParryAbility is ended with knocback, don't exit the state from: %s"), *GetClass()->GetName());
		return;
	}
	else
	{
		BroadcastTransition(FGameplayTag(), nullptr, "Parry Ability ended with out knocback.");
	}
}

void UInComingAttackState::OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "ParryKnocback Ability is ended.");
}

void UInComingAttackState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	CleanupDelegates();
	UnBindTargetComingAttackEnd();
}

void UInComingAttackState::CleanupDelegates()
{
	if (IsValid(DamageSubsystem))
	{
		DamageSubsystem->OnDamageDealt.RemoveDynamic(this, &UInComingAttackState::OnDamageDealt);
	}

	if (IsValid(Enemy) && Enemy->GetTagDelegatesComponent())
	{
		Enemy->GetTagDelegatesComponent()->UnregisterAllDelegatesForObject(this);
	}

	if (IsValid(LastUsedParry) && ParryEndHandle.IsValid())
	{
		LastUsedParry->OnAbilityEnded.Remove(ParryEndHandle);
		ParryEndHandle.Reset();
		LastUsedParry = nullptr;
	}

	if (IsValid(LastUsedParryKnocback) && ParryKnockbackEndHandle.IsValid())
	{
		LastUsedParryKnocback->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
		LastUsedParryKnocback = nullptr;
	}
}

