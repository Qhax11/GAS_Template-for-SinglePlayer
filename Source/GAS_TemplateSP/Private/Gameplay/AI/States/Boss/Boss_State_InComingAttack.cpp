// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_InComingAttack.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionDataDodge.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/ComingAttackReaction/ComingAttackReactionData.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Movement/MovementSingleData.h"
#include "Gameplay/Abilities/InCombat/Attack/GA_MeleeAttackBase.h"

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

	return false;
}

void UBoss_State_InComingAttack::ActivateDodgeAbility(UComingAttackReactionData* SelectedBestReaction)
{
	if (!SelectedBestReaction) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: SelectedBestReaction is null."));
		return;
	}

	UComingAttackReactionDataDodge* DodgeReactionData = Cast<UComingAttackReactionDataDodge>(SelectedBestReaction);
	if (!DodgeReactionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: DodgeReactionData is null."));
		return;
	}

	UMovementSingleData* DodgeMovementAbilityData = DodgeReactionData->DodgeMovementAbilityData;
	if (!DodgeMovementAbilityData->MovementAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("State Manager: Dodge MovementAbilityClass is null."));
		return;
	}

	const bool ExecutionSucces = MovementManager->ExecuteReactionMovement(DodgeMovementAbilityData, InComingAttackStatePayload->AttackPayload);
	if (!ExecutionSucces) 
	{
		bool bExitRequestSucces = ExitRequest("Dodge Ability Cannot Executed");
		if (!bExitRequestSucces)
		{
			BindTargetComingAttackEnd();
		}
	}
}

void UBoss_State_InComingAttack::OnDodgeAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
	// Execution path if the function was already called on the Game Thread.
	UE_LOG(LogTemp, Warning, TEXT("State Manager: OnDodgeAbilityEnded entered."));
	ExitRequest("OnDodgeAbilityEnded");
}

void UBoss_State_InComingAttack::OnExit_Implementation()
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

	if (LastUsedDodgeAbility)
	{
		LastUsedDodgeAbility->OnAbilityEnded.RemoveAll(this);
		LastUsedDodgeAbility = nullptr;
	}
}
