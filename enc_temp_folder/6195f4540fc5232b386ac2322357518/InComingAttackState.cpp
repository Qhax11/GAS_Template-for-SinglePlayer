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
		LastComingAttackAbility = nullptr;
		ComingAttackEndHandle.Reset();
		return;
	}

	// Remove previous binding safely
	if (IsValid(LastComingAttackAbility) && ComingAttackEndHandle.IsValid())
	{
		LastComingAttackAbility->OnAbilityEnded.Remove(ComingAttackEndHandle);
		ComingAttackEndHandle.Reset();
	}

	ComingAttackEndHandle = ComingAttack->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnComingAttackAbilityEnded);
	LastComingAttackAbility = ComingAttack;
}

void UInComingAttackState::OnComingAttackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "ComingAttackAbility is ended.");
}

void UInComingAttackState::OnDamageDealt(const FDamageData& DamageData)
{
	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: OnDamageDealt entered from: %s"), *GetClass()->GetName());

	// If parry failed, we exit anyway; no need to unbind first.
	if (!DamageData.bParrySucces)
	{
		UnBindTargetComingAttackEnd();
		BroadcastTransition(FGameplayTag(), nullptr, "Parry is failed");
		return;
	}

	// parry success: now we take over exit flow (knockback / parry end), so unbind coming attack end.
	UnBindTargetComingAttackEnd();
	ExecuteParryKnocback(DamageData);
}

void UInComingAttackState::UnBindTargetComingAttackEnd()
{
	if (IsValid(LastComingAttackAbility) && ComingAttackEndHandle.IsValid())
	{
		LastComingAttackAbility->OnAbilityEnded.Remove(ComingAttackEndHandle);
		ComingAttackEndHandle.Reset();
	}

	LastComingAttackAbility = nullptr;
}

void UInComingAttackState::ExecuteParry(const UComingAttackReactionData* BestComingAttackReaction)
{
	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: MakeParryAbility entered."));

	if (LastUsedParryAbility && LastUsedParryAbility->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: LastUsedParryAbility is active from: %s"), *GetClass()->GetName());
		if (LastUsedParryAbility)
		{
			EnemyASC->CancelAbilityHandle(LastUsedParryAbility->GetCurrentAbilitySpecHandle());
			//LastUsedParryAbility->EndAbilityManually();
		}
	}

	const bool bIsInActiveAttackPhase = EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_Attack);
	if (bIsInActiveAttackPhase)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: bIsInActiveAttackPhase TRUE."));
	}

	FaceTargetBeforeParry();

	UGAS_GameplayAbilityBase* ActivatedParryAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(EnemyParryAbilityClass);
	if (ActivatedParryAbility && ActivatedParryAbility->IsActive())
	{
		if (IsValid(LastUsedParryAbility) && ParryEndHandle.IsValid())
		{
			LastUsedParryAbility->OnAbilityEnded.Remove(ParryEndHandle);
			ParryEndHandle.Reset();
		}

		ParryEndHandle = ActivatedParryAbility->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: MakeParryAbility executed from: %s"), *GetClass()->GetName());
	}

	LastUsedParryAbility = ActivatedParryAbility;
}

void UInComingAttackState::ExecuteParryKnocback(const FDamageData& DamageData)
{
	// default: not waiting unless we successfully start knockback
	bWaitingForParryKnockbackEnd = false;

	FGameplayEventData Payload;
	Payload.EventTag = GAS_Tags::TAG_Gameplay_AbilityTriggerEvent_ParryKnockback;
	Payload.Instigator = DamageData.ExecCalculationParameters.SourceActor;
	Payload.Target = DamageData.ExecCalculationParameters.TargetActor;
	Payload.ContextHandle = DamageData.ExecCalculationParameters.GetSpec().GetContext();
	Payload.InstigatorTags = DamageData.ExecCalculationParameters.GetSpec().CapturedSourceTags.GetActorTags();

	UGAS_GameplayAbilityBase* Knockback = EnemyASC->TryActivateAbilityByClassWithEventData(EnemyParryKnocbackAbilityClass, Payload);

	// If activation failed, do NOT wait for an end delegate that will never fire.
	if (!IsValid(Knockback) || !Knockback->IsActive())
	{
		LastUsedParryKnocback = Knockback;
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ParryKnocback activation FAILED -> exiting state"));

		BroadcastTransition(FGameplayTag(), nullptr, "ParryKnockback activation failed.");
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: ParryKnocback is activated"));

	// Remove previous knockback end binding safely
	if (IsValid(LastUsedParryKnocback) && ParryKnockbackEndHandle.IsValid())
	{
		LastUsedParryKnocback->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
	}

	ParryKnockbackEndHandle = Knockback->OnAbilityEnded.AddUObject(this, &UInComingAttackState::OnParryKnocbackAbilityEnded);

	LastUsedParryKnocback = Knockback;
	bWaitingForParryKnockbackEnd = true;
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
	// ONLY block exit if we truly have an active knockback ability to wait for.
	const bool bKnockbackActive = IsValid(LastUsedParryKnocback) && LastUsedParryKnocback->IsActive();
	if (bWaitingForParryKnockbackEnd && bKnockbackActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UInComingAttackState: Parry ended, waiting knockback end."));
		return;
	}

	BroadcastTransition(FGameplayTag(), nullptr, "Parry Ability ended with out knocback.");
}

void UInComingAttackState::OnParryKnocbackAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	bWaitingForParryKnockbackEnd = false;
	BroadcastTransition(FGameplayTag(), nullptr, "ParryKnockback Ability is ended.");
}

void UInComingAttackState::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	bWaitingForParryKnockbackEnd = false;
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

	if (IsValid(LastUsedParryKnocback) && ParryKnockbackEndHandle.IsValid())
	{
		LastUsedParryKnocback->OnAbilityEnded.Remove(ParryKnockbackEndHandle);
		ParryKnockbackEndHandle.Reset();
		LastUsedParryKnocback = nullptr;
	}
}

