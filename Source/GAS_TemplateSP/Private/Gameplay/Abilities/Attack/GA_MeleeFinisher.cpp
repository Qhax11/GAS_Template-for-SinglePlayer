// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Attack/GA_MeleeFinisher.h"

UGA_MeleeFinisher::UGA_MeleeFinisher()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
}

UAnimMontage* UGA_MeleeFinisher::GetRandomHeroMontage(FGameplayTag& OutChosenTag)
{
    if (!FinisherAttackData || FinisherAttackData->FinisherAttackPairs.Num() == 0)
        return nullptr;

    int32 RandomIndex = FMath::RandRange(0, FinisherAttackData->FinisherAttackPairs.Num() - 1);
    const FMeleeFinisherAttackPair& ChosenPair = FinisherAttackData->FinisherAttackPairs[RandomIndex];

    OutChosenTag = ChosenPair.FinisherTag;
    return ChosenPair.FinisherAttackMontage;
}
