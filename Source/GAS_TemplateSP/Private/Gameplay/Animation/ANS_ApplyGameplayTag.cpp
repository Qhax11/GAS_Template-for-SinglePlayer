// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/ANS_ApplyGameplayTag.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemGlobals.h>


void UANS_ApplyGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());

		if (ASC && TagToApply.IsValid())
		{
			ASC->AddLooseGameplayTag(TagToApply);
		}
	}
}

void UANS_ApplyGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());

		if (ASC && TagToApply.IsValid())
		{
			ASC->RemoveLooseGameplayTag(TagToApply);
		}
	}
}

FString UANS_ApplyGameplayTag::GetNotifyName_Implementation() const
{
	if (!TagToApply.IsValid())
	{
		return TEXT("TAG MISSING!");
	}

	FString FullTagString = TagToApply.ToString();
	int32 FirstDotIndex = INDEX_NONE;

	// 2. Attempt to find the index of the first dot character ('.').
	// This dot typically separates the root category (like "Gameplay") from the rest.
	if (FullTagString.FindChar(TEXT('.'), FirstDotIndex))
	{
		// 3. Use RightChop to return the string starting AFTER the first dot.
		// +1 is added to the index to exclude the dot character itself.
		// Result: "State.Phase.Startup"
		return FullTagString.RightChop(FirstDotIndex + 1);
	}

	return FullTagString;
}


