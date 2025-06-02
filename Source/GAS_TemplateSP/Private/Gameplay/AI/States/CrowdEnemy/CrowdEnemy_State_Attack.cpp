// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/CrowdEnemy/CrowdEnemy_State_Attack.h"
#include "Gameplay/Abilities/Attack/GA_ComboMeleeAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"


void UCrowdEnemy_State_Attack::OnEnter_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CrowdEnemy Attack State has been enter"));

	bStateFinished = false;

	UClass* AttackClass = GetSelectedAttackAbilityData().AbilityClass;

	if (AttackClass && AttackClass->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
	{
		TSubclassOf<UGA_ComboMeleeAttack> ComboAttackClass = AttackClass;
		Enemy->GetEnemyMeleeComboManagerComponent()->StartComboChainWithClass(ComboAttackClass);

		if (!Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UCrowdEnemy_State_Attack::OnComboChaindEnded))
		{
			Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.AddDynamic(this, &UCrowdEnemy_State_Attack::OnComboChaindEnded);
		}
	}
}

void UCrowdEnemy_State_Attack::OnExit_Implementation()
{
	if (Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.IsAlreadyBound(this, &UCrowdEnemy_State_Attack::OnComboChaindEnded))
	{
		Enemy->GetEnemyMeleeComboManagerComponent()->OnComboEnded.RemoveDynamic(this, &UCrowdEnemy_State_Attack::OnComboChaindEnded);
	}
}

void UCrowdEnemy_State_Attack::OnComboChaindEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("CrowdEnemy OnComboChaindEnded"));
	ExitRequest();
}
