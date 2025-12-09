// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/AN_SendGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemGlobals.h>

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !EventTag.IsValid())
	{
		return;
	}

	FGameplayEventData EventData;
	ASC->HandleGameplayEvent(EventTag, &EventData);
}

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	if (!EventTag.IsValid())
	{
		return TEXT("AN_SendGameplayEvent: TAG MISSING!");
	}

	const FString Full = EventTag.ToString();
	const FString Prefix = TEXT("Gameplay.Event.AnimNotify.");

	// Remove fixed prefix
	if (Full.StartsWith(Prefix))
	{
		FString Trimmed = Full.RightChop(Prefix.Len());
		return TEXT("AN_SendGameplayEvent: ") + Trimmed;
	}

	// Fallback: return whole tag after prefix
	return TEXT("AN_SendGameplayEvent: ") + Full;
}
