// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_Dormant.h"

UBoss_State_Dormant::UBoss_State_Dormant()
{
	StateTag = GAS_Tags::TAG_AI_State_Boss_Dormant;
}

void UBoss_State_Dormant::OnExit_Implementation()
{
	EnemyASC->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_State_Rotation_LockTowardsTarget);
	Super::OnExit_Implementation();
}
