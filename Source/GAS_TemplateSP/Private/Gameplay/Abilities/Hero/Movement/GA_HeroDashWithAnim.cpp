// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroDashWithAnim.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

UGA_HeroDashWithAnim::UGA_HeroDashWithAnim()
{
    AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement_Dash);

    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead);

    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash);
}

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
    if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        return GAS_Tags::TAG_Gameplay_Direction_Forward;
    }

    const float X = Input.X;
    const float Y = Input.Y;

    // No input → forward
    if (X == 0.f && Y == 0.f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Forward;
    }

    const float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(X, Y));

    if (AngleDeg >= -45.f && AngleDeg <= 45.f) 
    {
        return GAS_Tags::TAG_Gameplay_Direction_Forward;
    }
    else if (AngleDeg > 45.f && AngleDeg < 135.f) 
    {
        return GAS_Tags::TAG_Gameplay_Direction_Right;
    }
    else if (AngleDeg >= 135.f || AngleDeg <= -135.f) 
    {
        return GAS_Tags::TAG_Gameplay_Direction_Backward;
    }
    else
    {
        return GAS_Tags::TAG_Gameplay_Direction_Left;
    }

}
