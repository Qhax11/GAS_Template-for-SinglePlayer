// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroDashWithAnim.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

void UGA_HeroDashWithAnim::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
    HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
    if (!HeroBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in: %s"), *GetName());
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    UAC_HeroControl* HeroControlComponent = HeroBase->GetHeroControlComponent();
    if (!HeroControlComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetHeroControlComponent is null in: %s"), *GetName());
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    if (!InputDirectionToDodgeMontageAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputDirectionToDodgeMontageAsset is null in: %s"), *GetName());
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    const FVector2D& HeroLastMovementInput = HeroControlComponent->LastMovementInput;

    const FGameplayTag InputDirectionTag = GetDirectionTagFromInput(HeroLastMovementInput);     

    UAnimMontage* FindedDodgeMontage = InputDirectionToDodgeMontageAsset->FindDodgetMontage(InputDirectionTag);
    if (!FindedDodgeMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("No montage found for direction tag: %s"), *InputDirectionTag.ToString());
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    DirectionTag = InputDirectionTag;
    AnimMontage = FindedDodgeMontage;

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FGameplayTag UGA_HeroDashWithAnim::GetDirectionTagFromInput(const FVector2D& Input) const
{
    const float X = Input.X;
    const float Y = Input.Y;

    // No input → forward
    if (X == 0.f && Y == 0.f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Forward;
    }

    const float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(X, Y));

    // Forward
    if (AngleDeg >= -45.f && AngleDeg <= 45.f)
        return GAS_Tags::TAG_Gameplay_Direction_Forward;

    // Right
    if (AngleDeg > 45.f && AngleDeg < 135.f)
        return GAS_Tags::TAG_Gameplay_Direction_Right;

    // Backward
    if (AngleDeg >= 135.f || AngleDeg <= -135.f)
        return GAS_Tags::TAG_Gameplay_Direction_Backward;

    // Left
    return GAS_Tags::TAG_Gameplay_Direction_Left;
}
