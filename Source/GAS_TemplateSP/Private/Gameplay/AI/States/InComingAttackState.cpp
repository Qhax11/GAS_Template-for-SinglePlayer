// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/InComingAttackState.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Parry.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Dodge.h"
#include "Gameplay/Abilities/GA_TakeDamageBase.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBase.h"


void UInComingAttackState::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
	EnemyASC->AbilityActivatedCallbacks.AddUObject(this, &UInComingAttackState::OnTakeDamageAbilityActivated);
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
	UBDS_ComingAttackReactionBase* SelectedBestReaction = BehaviorDecisionComponent->LastSelectedComingAttackReaction;
	if (!SelectedBestReaction) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedBestReaction is null in: %s"), *GetName());
		return;
	}

	switch (SelectedBestReaction->ReactionType)
	{
	case EComingAttackReaction::Parry:
		MakeParryAbility(SelectedBestReaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeParryAbility"));
		break;

	case EComingAttackReaction::Dodge:
		ActivateDodgeAbility(SelectedBestReaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered ActivateDodgeAbility"));
		break;

	case EComingAttackReaction::TakeDamage:
		MakeTakeDamage(SelectedBestReaction);
		UE_LOG(LogTemp, Warning, TEXT("Triggered MakeTakeDamage"));
		break;
	}
}

void UInComingAttackState::MakeTakeDamage(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
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

void UInComingAttackState::OnTakeDamageAbilityActivated(UGameplayAbility* Ability)
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

void UInComingAttackState::MakeParryAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	BDS_Parry = Cast<UBDS_ComingAttackReaction_Parry>(BestComingAttackReaction);
	if (!BDS_Parry)
	{
		UE_LOG(LogTemp, Warning, TEXT("BDS_Parry is null in: %s"), *GetName());
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

	if (BDS_Parry->bCounterImmediatelyAfterParry)
	{
		FTimerHandle DelayHandle;
		Enemy->GetWorldTimerManager().SetTimer(DelayHandle, [this]()
			{
				ExitRequest();
			}, 0.1f, false); 
	}
}

void UInComingAttackState::OnParryKnocbackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	UE_LOG(LogTemp, Warning, TEXT("ParryKnockback tag removed. Exiting."));
	ExitRequest();
}

void UInComingAttackState::ActivateDodgeAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	const UBDS_ComingAttackReaction_Dodge* BDS_Dodge = Cast<UBDS_ComingAttackReaction_Dodge>(BestComingAttackReaction);
	if (!BDS_Dodge)
	{
		UE_LOG(LogTemp, Warning, TEXT("BDS_Dodge is null in: %s"), *GetName());
		return;
	}

	if (Enemy && HeroTarget)
	{
		FVector StartLocation = Enemy->GetActorLocation();
		FVector TargetLocation = HeroTarget->GetActorLocation();

		// Yalnızca yaw (yatay) rotasyonu hesapla, pitch ve roll sabit kalsın
		FRotator LookAtRotation = (TargetLocation - StartLocation).Rotation();
		LookAtRotation.Pitch = 0.f;
		LookAtRotation.Roll = 0.f;

		Enemy->SetActorRotation(LookAtRotation);
	}

	Enemy->GetEnemyMeleeComboManagerComponent()->StopCombo();
	Enemy->GetEnemyMovementManagerComponent()->StopMovementAbilities();

	FGameplayEventData GameplayEventData = FGameplayEventData();
	GameplayEventData.InstigatorTags.AddTag(BDS_Dodge->DodgeMovementAbilityData.ResolvedDirectionTag);
	GameplayEventData.EventTag = BDS_Dodge->DodgeMovementAbilityData.AbilityTriggerTag;
	GameplayEventData.EventMagnitude = BDS_Dodge->DodgeMovementAbilityData.AbilityEventMagnitude;

	UGAS_GameplayAbilityBase* ActivatedAbility =
	EnemyASC->TryActivateAbilityByClassWithEventData(BDS_Dodge->DodgeMovementAbilityData.MovementAbilityClass, GameplayEventData);
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



