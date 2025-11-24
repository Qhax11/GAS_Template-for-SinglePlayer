// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/ANS_ModifyGameplayTag.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemGlobals.h>


void UANS_ModifyGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !TagToApply.IsValid())
	{
		return;
	}

	if (bRemoveThenAdd) 
	{
		ASC->RemoveLooseGameplayTag(TagToApply, 100);
	}
	else
	{
		ASC->AddLooseGameplayTag(TagToApply);
	}
}

void UANS_ModifyGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !TagToApply.IsValid())
	{
		return;
	}

	if (bRemoveThenAdd)
	{
		ASC->AddLooseGameplayTag(TagToApply);
	}
	else
	{
		ASC->RemoveLooseGameplayTag(TagToApply, 100);
	}
}

FString UANS_ModifyGameplayTag::GetNotifyName_Implementation() const
{
	if (!TagToApply.IsValid())
	{
		return TEXT("ModifyGameplayTag: TAG MISSING!");
	}

	FString FullTagString = TagToApply.ToString();
	int32 FirstDotIndex = INDEX_NONE;

	FString Trimmed = FullTagString;

	// Try trimming
	if (FullTagString.FindChar(TEXT('.'), FirstDotIndex))
	{
		Trimmed = FullTagString.RightChop(FirstDotIndex + 1);
	}

	// Always add prefix
	return TEXT("ModifyGameplayTag: ") + Trimmed;
}


