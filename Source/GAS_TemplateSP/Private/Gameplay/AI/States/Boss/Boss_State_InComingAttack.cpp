// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_InComingAttack.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/BDS_ComingAttackReaction_Dodge.h"

void UBoss_State_InComingAttack::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UBoss_State_InComingAttack::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();
}

bool UBoss_State_InComingAttack::SelectAndMakeInComingAttackReaction()
{
	if (Super::SelectAndMakeInComingAttackReaction())
	{
		return true;
	}

	UBDS_ComingAttackReactionBase* SelectedBestReaction = BehaviorDecisionComponent->LastSelectedComingAttackReaction;
	if (!SelectedBestReaction)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedBestReaction is null in: %s"), *GetName());
		return false;
	}

	if (SelectedBestReaction->ReactionType == EComingAttackReaction::Dodge) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: ActivateDodgeAbility entered."));
		ActivateDodgeAbility(SelectedBestReaction);
		return true;
	}

	return false;
}

void UBoss_State_InComingAttack::ActivateDodgeAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	const UBDS_ComingAttackReaction_Dodge* BDS_Dodge = Cast<UBDS_ComingAttackReaction_Dodge>(BestComingAttackReaction);
	if (!BDS_Dodge)
	{
		UE_LOG(LogTemp, Warning, TEXT("BDS_Dodge is null in: %s"), *GetName());
		UE_LOG(LogTemp, Warning, TEXT("State Manager: BDS_Dodge entered."));
		return;
	}

	if (Enemy && HeroTarget)
	{
		FVector StartLocation = Enemy->GetActorLocation();
		FVector TargetLocation = HeroTarget->GetActorLocation();

		// Yalnýzca yaw (yatay) rotasyonu hesapla, pitch ve roll sabit kalsýn
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

	UGAS_GameplayAbilityBase* ActivatedDodgeAbility =
		EnemyASC->TryActivateAbilityByClassWithEventData(BDS_Dodge->DodgeMovementAbilityData.MovementAbilityClass, GameplayEventData);
	if (ActivatedDodgeAbility && ActivatedDodgeAbility->IsActive())
	{
		if (!ActivatedDodgeAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded))
		{
			ActivatedDodgeAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded);
		}
		UE_LOG(LogTemp, Warning, TEXT("State Manager: ActivatedDodgeAbility entered."));
		LastUsedDodgeAbility = ActivatedDodgeAbility;
	}
	else
	{
		ExitRequest("Dodge Ability Cannot Executed");
	}
}

void UBoss_State_InComingAttack::OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	/*
	// The delegate can be triggered from a Worker Thread (e.g., via animation tasks).
// Critical state changes (State Manager/UObject changes) MUST be on the Game Thread.

	if (!IsInGameThread())
	{
		// KENDÝ FONKSÝYONUMUZU Game Thread'e ertelemek.
		TWeakObjectPtr<UBoss_State_InComingAttack> WeakThis(this);
		FAbilityEndedDataBP LocalData = DodgeAbilityEndedData; // Veriyi Worker Thread'den kopyala

		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([WeakThis, LocalData]()
				{
					if (UBoss_State_InComingAttack* Self = WeakThis.Get())
					{
						// FONKSÝYONUN KENDÝSÝNÝ Game Thread'de tekrar çaðýr.
						// (Recursion deðil, basitçe Game Thread'e geçiþ)
						UE_LOG(LogTemp, Warning, TEXT("State Manager: OnParryAbilityEnded deferred to Game Thread."));
						Self->OnDodgeAbilityEnded(LocalData);
					}
				}),
			TStatId(),
			nullptr,
			ENamedThreads::GameThread
		);
		return; // Worker Thread'den hemen çýk.
	}

	// ----------- BU NOKTADAN ÝTÝBAREN HER ZAMAN GAME THREAD'DEYÝZ -----------
	*/
	ExitRequest("OnDodgeAbilityEnded");

	// Execution path if the function was already called on the Game Thread.
	UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDodgeAbilityEnded entered."));
	ExitRequest("OnDodgeAbilityEnded");
}

void UBoss_State_InComingAttack::OnExit_Implementation()
{
	/*
	// Ýþ parçacýðý kontrolünü daha güvenli hale getirelim:
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UInComingAttackState> WeakThis(this);
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([WeakThis]()
				{
					if (UInComingAttackState* Self = WeakThis.Get())
					{
						// Game Thread'de OnExit'i güvenle tekrar çaðýr.
						UE_LOG(LogTemp, Warning, TEXT("State Manager: OnExit deferred to Game Thread."));
						Self->OnExit_Implementation();
					}
				}),
			TStatId(),
			nullptr,
			ENamedThreads::GameThread
		);
		return; // Worker Thread'den hemen çýk.
	}

	// ----------- BURADAN SONRA SADECE GAME THREAD'DEYÝZ -----------
	*/
	Super::OnExit_Implementation();

	if (LastUsedDodgeAbility)
	{
		if (LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded))
		{
			LastUsedDodgeAbility->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded);
		}
		LastUsedDodgeAbility = nullptr;
	}
}
