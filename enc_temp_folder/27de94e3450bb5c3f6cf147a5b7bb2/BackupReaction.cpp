// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/BackupReaction.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"

UBackupReaction::UBackupReaction()
{
	StateTag = GAS_Tags::TAG_AI_State_BackupReaction;
}

void UBackupReaction::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);

	UE_LOG(LogTemp, Log, TEXT("State: UBackupReaction: OnEnter called"));

	AGAS_CharacterBase* HeroCharacater = Cast<AGAS_CharacterBase>(HeroTarget);
	if (!HeroCharacater) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UBackupReaction: HeroCharacater is null!"));
		return;
	}

	UAC_TagDelegates* HeroTagDelegateComp = HeroCharacater->GetTagDelegatesComponent();
	if (!HeroTagDelegateComp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("State: UBackupReaction: HeroTagDelegateComp is null!"));
		return;
	}

	HeroTagDelegateComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Moving_Patrolling, EListenMode::OnRemoved).BindDynamic(this, &UBackupReaction::OnFinisherTagRemoved);
}

void UBackupReaction::OnFinisherTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
	BroadcastTransition(FGameplayTag(), nullptr, "FinisherTag is Removed.");
}
