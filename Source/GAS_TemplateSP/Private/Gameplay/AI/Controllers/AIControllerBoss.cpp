// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBoss.h"
#include "Gameplay/AI/Components/IntendHandler/AC_IntendHandlerBoss.h"

AAIControllerBoss::AAIControllerBoss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IntendHandlerBossComponent = CreateDefaultSubobject<UAC_IntendHandlerBoss>(TEXT("IntendHandlerBossComponent"));
}

void AAIControllerBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerBoss::TargetPreceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (bHasTargetBeenDetected)
	{
		return; 
	}

	Super::TargetPreceptionUpdated(Actor, Stimulus);
}

bool AAIControllerBoss::ShouldUpdateRotation() const
{
	bool BossInUnstoppableAttack = ControlledEnemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_UnstoppableAttack);
	if (BossInUnstoppableAttack) 
	{
		return false;
	}

	return Super::ShouldUpdateRotation();
}


