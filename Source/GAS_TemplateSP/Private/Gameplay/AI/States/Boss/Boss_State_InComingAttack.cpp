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

	UGAS_GameplayAbilityBase* ActivatedAbility =
		EnemyASC->TryActivateAbilityByClassWithEventData(BDS_Dodge->DodgeMovementAbilityData.MovementAbilityClass, GameplayEventData);
	if (ActivatedAbility)
	{
		if (!ActivatedAbility->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded))
		{
			ActivatedAbility->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded);
		}
		LastUsedDodgeAbility = ActivatedAbility;
	}
}

void UBoss_State_InComingAttack::OnDodgeAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
	ExitRequest("OnDodgeAbilityEnded");
}

void UBoss_State_InComingAttack::OnExit_Implementation()
{
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
