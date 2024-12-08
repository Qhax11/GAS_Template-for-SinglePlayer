// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/GA_HeroHologram.h"

void UGA_HeroHologram::OnGameplayEventValidData(const FGameplayAbilityTargetDataHandle& Data)
{
    for (TSharedPtr<FGameplayAbilityTargetData> TargetData : Data.Data)
    {
        const FGameplayAbilityTargetData_LocationInfo* LocationInfo = static_cast<FGameplayAbilityTargetData_LocationInfo*>(TargetData.Get());
        if (LocationInfo)
        {
            FVector ExtractedLocation = LocationInfo->TargetLocation.GetTargetingTransform().GetLocation();
            GetAvatarActorFromActorInfo()->SetActorLocation(ExtractedLocation);
        }
    }

    Super::OnGameplayEventValidData(Data);
}
