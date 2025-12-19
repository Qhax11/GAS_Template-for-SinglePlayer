// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_InComingAttack.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionDataDodge.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"

void UBoss_State_InComingAttack::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);
}

void UBoss_State_InComingAttack::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);
}

bool UBoss_State_InComingAttack::SelectAndExecuteReaction(UComingAttackReactionData* SelectedReactionData)
{
	// If it's success on Super we don't have to do anything.
	if (Super::SelectAndExecuteReaction(SelectedReactionData))
	{
		return true;
	}

	if (!SelectedReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedReactionData is invalid in: %s"), *GetName());
		return false;
	}

	if (SelectedReactionData->ReactionType == EReactionType::Dodge)
	{
		ActivateDodgeAbility(SelectedReactionData);
		return true;
	}

	return false;
}

void UBoss_State_InComingAttack::ActivateDodgeAbility(UComingAttackReactionData* SelectedBestReaction)
{
	if (!SelectedBestReaction) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UBoss_State_InComingAttack: SelectedBestReaction is null."));
		return;
	}

	UComingAttackReactionDataDodge* DodgeReactionData = Cast<UComingAttackReactionDataDodge>(SelectedBestReaction);
	if (!DodgeReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBoss_State_InComingAttack: DodgeReactionData is null."));
		return;
	}

	UMovementSingleData* DodgeMovementAbilityData = DodgeReactionData->DodgeMovementAbilityData;
	if (!DodgeMovementAbilityData->MovementAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBoss_State_InComingAttack: Dodge MovementAbilityClass is null."));
		return;
	}

	UGAS_GameplayAbilityBase* ReactionMovement = MovementManager->ExecuteReactionMovement(DodgeMovementAbilityData, InComingAttackStatePayload->AttackPayload);
	if (ReactionMovement)
	{
		ReactionMovement->OnAbilityEnded.RemoveAll(this);
		ReactionMovement->OnAbilityEnded.AddUObject(this, &UBoss_State_InComingAttack::OnDodgeAbilityEnded);
	}
	else
	{
		bool bExitRequestSucces = ExitRequest("Dodge Ability Cannot Executed");
		if (!bExitRequestSucces)
		{
			BindTargetComingAttackEnd();
		}
	}
}

void UBoss_State_InComingAttack::OnDodgeAbilityEnded(const FCustomAbilityEndedData& ReactionMovementEndedData)
{
	// Execution path if the function was already called on the Game Thread.
	UE_LOG(LogTemp, Warning, TEXT("UBoss_State_InComingAttack: OnDodgeAbilityEnded entered."));
	ExitRequest("OnDodgeAbilityEnded");
}

void UBoss_State_InComingAttack::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedDodgeAbility)
	{
		LastUsedDodgeAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedDodgeAbility = nullptr;
	}
}
