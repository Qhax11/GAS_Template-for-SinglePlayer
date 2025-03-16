// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Controllers/AIControllerBoss.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/AC_TagDelegates.h"

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

	if (AGAS_CharacterBase* TargetCharacter = Cast<AGAS_CharacterBase>(Actor)) 
	{
		if (UAC_TagDelegates* TargetCharacterTagDelegatesComp = TargetCharacter->GetTagDelegatesComponent())
		{
			TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo1, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBoss::OnPlayerStartedAttackTagAdded);
			TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo2, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBoss::OnPlayerStartedAttackTagAdded);
			TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeCombo3, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBoss::OnPlayerStartedAttackTagAdded);
		}
	}

	if (AGAS_CharacterBase* OwnerCharacter = Cast<AGAS_CharacterBase>(GetPawn()))
	{
		if (UAC_TagDelegates* TargetCharacterTagDelegatesComp = OwnerCharacter->GetTagDelegatesComponent())
		{
			TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_DeadWithFinisher, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBoss::OnDeadWithFinisherTagAdded);
			TargetCharacterTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Vulnerable, EListenMode::OnAdded).BindDynamic(this, &AAIControllerBoss::OnVulnerableTagAdded);
		}
	}
	
}

void AAIControllerBoss::OnPlayerStartedAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_PlayerStartedAttack, FConstStructView::Make(FMyStateTreePayload(23, FVector(100, 200, 300))));
}

void AAIControllerBoss::OnDeadWithFinisherTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_DeadWithFinisher);
}

void AAIControllerBoss::OnVulnerableTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	StateTreeAIComponent->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_Vulnerable);
}
