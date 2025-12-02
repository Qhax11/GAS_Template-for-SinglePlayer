// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_InComingAttack.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/ComingAttackReactionDataDodge.h"
#include "Gameplay/AI/BehaviorDecision/Services/ComingAttackReaction/ComingAttackReactionData.h"

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
	/*
	UComingAttackReactionData* SelectedBestReaction = BehaviorDecisionComponent->LastSelectedComingAttackReaction;
	if (SelectedBestReaction->ReactionType == EComingAttackReaction::Dodge) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: ActivateDodgeAbility entered."));
		ActivateDodgeAbility(SelectedBestReaction);
		return true;
	}
	*/
	return false;
}

void UBoss_State_InComingAttack::ActivateDodgeAbility(const UBDS_ComingAttackReactionBase* BestComingAttackReaction)
{
	/*
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
	//GameplayEventData.InstigatorTags.AddTag(BDS_Dodge->DodgeMovementAbilityData.ResolvedDirectionTag);
	//GameplayEventData.EventTag = BDS_Dodge->DodgeMovementAbilityData.AbilityTriggerTag;
	//GameplayEventData.EventMagnitude = BDS_Dodge->DodgeMovementAbilityData.AbilityEventMagnitude;

	UGAS_GameplayAbilityBase* ActivatedDodgeAbility =
		EnemyASC->TryActivateAbilityByClassWithEventData(BDS_Dodge->DodgeMovementAbilityData.MovementAbilityClass, GameplayEventData);
	if (ActivatedDodgeAbility && ActivatedDodgeAbility->IsActive())
	{
		ActivatedDodgeAbility->OnAbilityEnded.RemoveAll(this);
		ActivatedDodgeAbility->OnAbilityEnded.AddUObject(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded);
		UE_LOG(LogTemp, Warning, TEXT("State Manager: ActivatedDodgeAbility entered."));
		LastUsedDodgeAbility = ActivatedDodgeAbility;
	}
	else
	{
		bool bExitRequestSucces = ExitRequest("Dodge Ability Cannot Executed");
		if (!bExitRequestSucces)
		{
			BindTargetComingAttackEnd();
		}
	}
	*/
}

void UBoss_State_InComingAttack::OnDodgeAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
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

	if (LastUsedDodgeAbility)
	{
		LastUsedDodgeAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedDodgeAbility = nullptr;
	}
}
