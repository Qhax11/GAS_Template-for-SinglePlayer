// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBoss.h"

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

bool AAIControllerBoss::RegisterTags(AGAS_CharacterBase* TargetCharacter)
{
	if (!Super::RegisterTags(TargetCharacter)) 
	{
		return false;
	}

	return true;
}

/*
void AAIControllerBoss::OnPlayerStartedAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_PlayerStartedAttack, FConstStructView::Make(FMyStateTreePayload(23, FVector(100, 200, 300))));
}
*/
void AAIControllerBoss::OnDeadWithFinisherTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}


