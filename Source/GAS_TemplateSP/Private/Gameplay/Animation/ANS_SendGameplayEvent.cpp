// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/ANS_SendGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemGlobals.h>
#include "Gameplay/Tags/GAS_Tags.h"

UANS_SendGameplayEvent::UANS_SendGameplayEvent()
{
	EventTagStart = GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Start;
	EventTagContinue = GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_Continue;
	EventTagEnd = GAS_Tags::TAG_Gameplay_Event_AnimNotifyState_AttackTrace_End;
}

void UANS_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !EventTagStart.IsValid())
	{
		return;
	}

	FGameplayEventData EventData;
	ASC->HandleGameplayEvent(EventTagStart, &EventData);
}

void UANS_SendGameplayEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !EventTagContinue.IsValid())
	{
		return;
	}

	FGameplayEventData EventData;
	ASC->HandleGameplayEvent(EventTagContinue, &EventData);
}

void UANS_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !EventTagEnd.IsValid())
	{
		return;
	}

	FGameplayEventData EventData;
	ASC->HandleGameplayEvent(EventTagEnd, &EventData);
}

FString UANS_SendGameplayEvent::GetNotifyName_Implementation() const
{
	TArray<FGameplayTag> TagsToShow;
	if (EventTagStart.IsValid())   TagsToShow.Add(EventTagStart);
	if (EventTagContinue.IsValid()) TagsToShow.Add(EventTagContinue);
	if (EventTagEnd.IsValid())     TagsToShow.Add(EventTagEnd);

	if (TagsToShow.Num() == 0)
	{
		return TEXT("ANS_SendGameplayEvent: TAGS MISSING!");
	}

	FString Result = TEXT("ANS_SendGameplayEvent: ");
	for (int32 i = 0; i < TagsToShow.Num(); ++i)
	{
		FString TagString = TagsToShow[i].ToString();
		const FString Prefix = TEXT("Gameplay.Event.AnimNotify.");
		if (TagString.StartsWith(Prefix))
		{
			TagString = TagString.RightChop(Prefix.Len());
		}

		Result += TagString;
		if (i < TagsToShow.Num() - 1)
		{
			Result += TEXT(", ");
		}
	}

	return Result;
}