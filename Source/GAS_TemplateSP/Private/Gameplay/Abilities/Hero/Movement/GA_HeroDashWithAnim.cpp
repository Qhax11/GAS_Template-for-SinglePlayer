// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroDashWithAnim.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"

UGA_HeroDashWithAnim::UGA_HeroDashWithAnim()
{
    AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement_Dash);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Phase_Active);
    ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash);

    WaitForEventTag.AddTag(GAS_Tags::TAG_Gameplay_Event_AnimNotify_Rotation_Lock);

    // Skip ability cost on perfect dodge. This ability is triggered as the
    // perfect-dodge follow-up, so resource cost should not be committed here.
    bApplyCommit = false;

    // Keep the dash state active slightly past montage end.
    // UGA_HeroRouterAttack checks for the dash tag to decide whether it should trigger
    // the dash-follow-up attack (DodgeRecoveryAttack). The delayed end ensures the tag
    // is still present during this decision window.
    MontageEndPolicy = EMontageEndPolicy::EndWithDelay;
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

    WaitOneFrameTask = UAT_WaitOneFrame::WaitOneFrame(this);
    if (!WaitOneFrameTask)
    {
        UE_LOG(LogTemp, Warning, TEXT("WaitOneFrameTask is null in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
        return;
    }
    WaitOneFrameTask->OnFinished.AddDynamic(this, &UGA_HeroDashWithAnim::OnAfterFrame);
    WaitOneFrameTask->ReadyForActivation();

    GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune);

    WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.3f);
    if (!WaitDelayTask)
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune);
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }
    WaitDelayTask->OnFinish.AddDynamic(this, &UGA_HeroDashWithAnim::RemoveDamageImmuneTag);
    WaitDelayTask->ReadyForActivation();

    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Window_Perfect)) 
    {
        OnPerfectDodgeReceivedBP();
    }
    else
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }
    }

}

void UGA_HeroDashWithAnim::OnAfterFrame()
{
    if (!HeroBase || !HeroControlComponent || !InputDirectionToDodgeMontageAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase or HeroControlComponent is null"));
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    if (GetAbilitySystemComponentFromActorInfo()->HasAnyMatchingGameplayTags(ActivationBlockedTags))
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
    Super::ActivateAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);
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

    if (AngleDeg >= -22.5f && AngleDeg <= 22.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Forward;
    }
    else if (AngleDeg > 22.5f && AngleDeg <= 67.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_ForwardRight;
    }
    else if (AngleDeg > 67.5f && AngleDeg <= 112.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Right;
    }
    else if (AngleDeg > 112.5f && AngleDeg <= 157.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_BackwardRight;
    }
    else if (AngleDeg > 157.5f || AngleDeg <= -157.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Backward;
    }
    else if (AngleDeg > -157.5f && AngleDeg <= -112.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_BackwardLeft;
    }
    else if (AngleDeg > -112.5f && AngleDeg <= -67.5f)
    {
        return GAS_Tags::TAG_Gameplay_Direction_Left;
    }
    else // -67.5f < AngleDeg < -22.5f
    {
        return GAS_Tags::TAG_Gameplay_Direction_ForwardLeft;
    }
}

FVector UGA_HeroDashWithAnim::CalculateMotionWarpingLocation() const
{
    if (!HeroBase)
    {
        return FVector::ZeroVector;
    }

    // Target locked modda DirectionTag'e göre hareket et
    if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
        GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked))
    {
        // Parent class'ın DirectionTag bazlı hesaplamasını kullan
        return Super::CalculateMotionWarpingLocation();
    }

    // Target locked değilse input yönüne göre hareket et
    FVector OwnerLocation = HeroBase->GetActorLocation();
    FVector MovementInput = HeroBase->GetCharacterMovement()->GetLastInputVector();

    if (MovementInput.IsNearlyZero())
    {
        return OwnerLocation + HeroBase->GetActorForwardVector() * MotionWarpingDistance;
    }

    FVector Direction = MovementInput.GetSafeNormal();

    // Güvenlik kontrolü ekle
    float Distance = (Direction * MotionWarpingDistance).Size();
    if (Distance < 10.0f) // Minimum 10 cm
    {
        return OwnerLocation + HeroBase->GetActorForwardVector() * MotionWarpingDistance;
    }

    return OwnerLocation + Direction * MotionWarpingDistance;
}

void UGA_HeroDashWithAnim::RemoveDamageImmuneTag()
{
    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune, 100);
}

void UGA_HeroDashWithAnim::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnEventReceived(EventTag, EventData);

    // During dash the character is allowed to rotate freely for a brief moment,
    // but once this notify triggers, rotation to movement is disabled to 
    // ensure consistent forward dash behavior.
    if (EventTag == GAS_Tags::TAG_Gameplay_Event_AnimNotify_Rotation_Lock)
    {
        HeroControlComponent->bOrientRotationToMovement = false;
    }
}

void UGA_HeroDashWithAnim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GAS_Tags::TAG_Gameplay_DamageImmune, 100);

    if (IsValid(WaitOneFrameTask))
    {
        WaitOneFrameTask->EndTask();
        WaitOneFrameTask = nullptr;
    }

    bool IsHeroLocked = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_TargetLockSystem_Hero_TargetLocked);
    bool IsHeroMeleeAttack = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_MeleeAttack);
    if(!IsHeroLocked && !IsHeroMeleeAttack)
    {
        HeroControlComponent->bOrientRotationToMovement = true;
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

