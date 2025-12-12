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
		UE_LOG(LogTemp, Warning, TEXT("EnterPayload is invalid in: %s"), *GetName());
		return false;
	}

	TSharedPtr<FTakeHitStatePayload> TakeHitPayload = StaticCastSharedPtr<FTakeHitStatePayload>(EnterPayload);
	if (!TakeHitPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeHitPayload is invalid in: %s"), *GetName());
		return false;
	}

	const bool bEnemyUnstoppable = EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);
	const bool bHeroCanInterrupt = HeroTargetASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_CanInterruptUnstoppableAttack);

	// Eðer unstoppable deðilse ? gir
	// Eðer unstoppable ama hero interrupt ediyorsa ? yine gir
	return !bEnemyUnstoppable || bHeroCanInterrupt;
}

void UTakeHitState::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	TSharedPtr<FTakeHitStatePayload> TakeHitPayload = StaticCastSharedPtr<FTakeHitStatePayload>(EnterPayload);
	if (!TakeHitPayload.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("InComingAttackStatePayload is invalid in: %s"), *GetName());
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

	if (LastUsedTakeDamageAbility && EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_Phase_Active_TakeHit))
	{
		LastUsedTakeDamageAbility->EndAbilityManually();
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
		UE_LOG(LogTemp, Warning, TEXT("State Manager: TakeDamageAbility executed from: %s"), *GetClass()->GetName());
	}

	LastUsedTakeDamageAbility = TakeDamageAbility;
}

void UTakeHitState::OnTakeHitAbilityEnded(const FCustomAbilityEndedData& AbilityEndedData)
{
	ExitRequest("OnTakeHitAbilityEnded");
}

void UTakeHitState::OnExit_Implementation()
{
	Super::OnExit_Implementation();	

	UE_LOG(LogTemp, Warning, TEXT("UTakeHitState:: OnExit_Implementation entered."));

	if (EnemyTagDelegatesComp)
	{
		EnemyTagDelegatesComp->UnregisterAllDelegatesForObject(this);
	}

	if (IsValid(LastUsedTakeDamageAbility))
	{
		LastUsedTakeDamageAbility->OnAbilityEnded.RemoveAll(this);
	}
}

