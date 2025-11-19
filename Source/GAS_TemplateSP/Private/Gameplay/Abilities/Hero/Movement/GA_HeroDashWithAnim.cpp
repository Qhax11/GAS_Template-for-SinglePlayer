// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroDashWithAnim.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"

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
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
        return;
    }

    HeroControlComponent = HeroBase->GetHeroControlComponent();
    if (!HeroControlComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetHeroControlComponent is null in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
        return;
    }

    if (!InputDirectionToDodgeMontageAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputDirectionToDodgeMontageAsset is null in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
        return;
    }

    // Wait one frame to ensure LastMovementInput is updated
    UAT_WaitOneFrame* Task = UAT_WaitOneFrame::WaitOneFrame(this);
    Task->OnFinished.AddDynamic(this, &UGA_HeroDashWithAnim::OnAfterFrame);
    Task->ReadyForActivation();
}

void UGA_HeroDashWithAnim::OnAfterFrame()
{
    if (!HeroControlComponent || !InputDirectionToDodgeMontageAsset)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    FGameplayTag InputDirectionTag;
    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        const FVector2D HeroLastMovementInput = HeroControlComponent->LastMovementInput;
        InputDirectionTag = GetDirectionTagFromInput(HeroLastMovementInput);
    }
    else
    {
        InputDirectionTag = GAS_Tags::TAG_Gameplay_Direction_Forward;
    }

    UAnimMontage* FoundDodgeMontage = InputDirectionToDodgeMontageAsset->FindDodgetMontage(InputDirectionTag);
    if (!FoundDodgeMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("No montage found for direction tag: %s"), *InputDirectionTag.ToString());
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    AnimMontage = FoundDodgeMontage;
    DirectionTag = InputDirectionTag;

    // Activate the ability now that input is read correctly
    Super::ActivateAbility(
        GetCurrentAbilitySpecHandle(),
        GetCurrentActorInfo(),
        GetCurrentActivationInfo(),
        nullptr
    );

}

FVector UGA_HeroDashWithAnim::CalculateMotionWarpingLocation() const
{
    if (!HeroBase)
    {
        return FVector::ZeroVector;
    }

    FVector OwnerLocation = HeroBase->GetActorLocation();

    // Karakterin son input vektörü (world space, kamera-relative)
    FVector MovementInput = HeroBase->GetCharacterMovement()->GetLastInputVector();

    // Input yoksa varsayılan olarak ileri yönde warp
    if (MovementInput.IsNearlyZero())
    {
        return OwnerLocation + HeroBase->GetActorForwardVector() * MotionWarpingDistance;
    }

    // Input yönünü normalize et
    FVector Direction = MovementInput.GetSafeNormal();

    // Motion warping hedef konumu
    FVector TargetLocation = OwnerLocation + Direction * MotionWarpingDistance;
    return TargetLocation;
}

FGameplayTag UGA_HeroDashWithAnim::GetDirectionTagFromInput(const FVector2D& Input) const
{
    if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        //return GAS_Tags::TAG_Gameplay_Direction_Forward;
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

void UGA_HeroDashWithAnim::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnEventReceived(EventTag, EventData);

    if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Hero_LockRotation)
    {
        HeroControlComponent->bOrientRotationToMovement = false;
    }
}

void UGA_HeroDashWithAnim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if(!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        HeroControlComponent->bOrientRotationToMovement = true;
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

