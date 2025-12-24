// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/TakeHitState.h"
#include "Gameplay/Abilities/Enemy/GA_EnemyTakeDamage.h"


UTakeHitState::UTakeHitState()
{
	StateTag = GAS_Tags::TAG_AI_State_TakeHit;
}	

void UTakeHitState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

bool UTakeHitState::EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	if (!EnterPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UTakeHitState: EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	TSharedPtr<FTakeHitStatePayload> TakeHitPayload = StaticCastSharedPtr<FTakeHitStatePayload>(EnterPayload);
	if (!TakeHitPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UTakeHitState: TakeHitPayload is invalid in: %s"), *GetName());
		return false;
	}

	if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack))
	{
		UE_LOG(LogTemp, Log, TEXT("State: UTakeHitState: Enemy is on UnstoppableAttack, not allowed enter the TakeHitState"), *GetName());
		return false;
	}

	return true;
}

void UTakeHitState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	TSharedPtr<FTakeHitStatePayload> TakeHitPayload = StaticCastSharedPtr<FTakeHitStatePayload>(EnterPayload);
	if (!TakeHitPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UTakeHitState: InComingAttackStatePayload is invalid in: %s"), *GetName());
		return;
	}

	ExecuteTakeHit(TakeHitPayload);
}

void UTakeHitState::ExecuteTakeHit(TSharedPtr<FTakeHitStatePayload> TakeHitPayload)
{
	if (!EnemyASC)
	{
		return;
	}

	TriggerTakeHitAbility(TakeHitPayload);
}

void UTakeHitState::TriggerTakeHitAbility(TSharedPtr<FTakeHitStatePayload> TakeHitPayload)
{
	if (!EnemyTakeDamageAbilityClass || !EnemyASC)
	{
		return;
	}

	FDamageData DamageData = TakeHitPayload->DamageData;
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
		TakeDamageAbility->OnAbilityEnded.AddUObject(this, &UTakeHitState::OnTakeHitAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State: UTakeHitState: TakeDamageAbility executed from: %s"), *GetClass()->GetName());
	}

	LastUsedTakeDamageAbility = TakeDamageAbility;
}

void UTakeHitState::OnTakeHitAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData)
{
	BroadcastTransition(FGameplayTag(), nullptr, "TakeHit Ability is ended.");
}

void UTakeHitState::OnExit_Implementation()
{
	Super::OnExit_Implementation();	

	UE_LOG(LogTemp, Warning, TEXT("State: UTakeHitState: OnExit_Implementation entered."));

	if (EnemyTagDelegatesComp)
	{
		EnemyTagDelegatesComp->UnregisterAllDelegatesForObject(this);
	}

	if (IsValid(LastUsedTakeDamageAbility))
	{
		LastUsedTakeDamageAbility->OnAbilityEnded.RemoveAll(this);
	}
}

