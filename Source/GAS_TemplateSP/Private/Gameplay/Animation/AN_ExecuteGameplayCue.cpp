// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Animation/AN_ExecuteGameplayCue.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemGlobals.h>

void UAN_ExecuteGameplayCue::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC || !GameplayCueTag.IsValid())
	{
		return;
	}

	ASC->ExecuteGameplayCue(GameplayCueTag, FGameplayEffectContextHandle());
}

FString UAN_ExecuteGameplayCue::GetNotifyName_Implementation() const
{
	if (!GameplayCueTag.IsValid())
	{
		return TEXT("AN_ExecuteGameplayCue: TAG MISSING!");
	}

	const FString Full = GameplayCueTag.ToString();
	const FString Prefix = TEXT("GameplayCue");

	// Remove fixed prefix
	if (Full.StartsWith(Prefix))
	{
		FString Trimmed = Full.RightChop(Prefix.Len());
		return TEXT("AN_ExecuteGameplayCue: ") + Trimmed;
	}

	// Fallback: return whole tag after prefix
	return TEXT("AN_ExecuteGameplayCue: ") + Full;
}
