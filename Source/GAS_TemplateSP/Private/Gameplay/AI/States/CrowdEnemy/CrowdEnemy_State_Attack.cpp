// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/CrowdEnemy/CrowdEnemy_State_Attack.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"


void UCrowdEnemy_State_Attack::StateInitalize(const FStateInitParams& StateInitParams)
{
	Super::StateInitalize(StateInitParams);

	AICrowdEventManager = GetWorld()->GetGameInstance()->GetSubsystem<US_AICrowdEventManager>();
	if (!AICrowdEventManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager is null in: %s, can not initialize"), *GetName());
		return;
	}
}

void UCrowdEnemy_State_Attack::OnEnter_Implementation()
{
	Super::OnEnter_Implementation();

	AICrowdEventManager->ForceAddAttackIntender(EnemyASC);
}

void UCrowdEnemy_State_Attack::OnExit_Implementation()
{
	Super::OnExit_Implementation();
}

