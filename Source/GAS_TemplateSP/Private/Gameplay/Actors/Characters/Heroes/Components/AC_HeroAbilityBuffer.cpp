// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAbilityBuffer.h"

UAC_HeroAbilityBuffer::UAC_HeroAbilityBuffer()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroAbilityBuffer::BeginPlay()
{
    Super::BeginPlay();

    if (!HeroASC)
    {
        UE_LOG(LogTemp, Error, TEXT("HeroASC NULL!"));
        return;
    }

    HeroASC->AbilityFailedCallbacks.AddUObject(this, &UAC_HeroAbilityBuffer::OnAbilityFailed);
}

void UAC_HeroAbilityBuffer::OnAbilityFailed(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& TagExplaining)
{

    FString AllTags;
    // Container boþ deðilse yazalým
    for (const FGameplayTag& Tag : TagExplaining)
    {
        AllTags += Tag.GetTagName().ToString();
        AllTags += TEXT(", ");
    }

    if (AllTags.IsEmpty())
    {
        AllTags = TEXT("None");
    }
    else
    {
        AllTags.RemoveFromEnd(TEXT(", "));
    }

    UE_LOG(LogTemp, Error, TEXT("OnAbilityFailed, TagExplain: %s"), *AllTags);
}

