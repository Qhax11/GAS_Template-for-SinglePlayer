// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Movement/GA_HeroDashWithAnim.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Abilities/Tasks/AT_WaitOneFrame.h"
#include <Abilities/Tasks/AbilityTask_WaitGameplayEvent.h>

UGA_HeroDashWithAnim::UGA_HeroDashWithAnim()
{
    AbilityTags.AddTag(GAS_Tags::TAG_Gameplay_Ability_Movement_Dash);

    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_TakeDamage);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InCombat_Dead);
    ActivationBlockedTags.AddTag(GAS_Tags::TAG_Gameplay_State_InAir);

    // Giving effect insted of using ActivationOwnedTags for the perfect dodge check in damage exec calculation
   // ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_Moving_Dash);
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

    // 2. Event Bekleme Task'ini Oluştur
    UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this,
        GAS_Tags::TAG_Gameplay_Event_Trigger_Perfect_Dodge,
        nullptr, // Opsiyonel Target Actor
        true,    // Only Trigger Once (Bir kere perfect dodge olunca task bitsin mi? Genelde Evet)
        true     // Match Exact
    );

    // 3. Task Tetiklendiğinde Çalışacak Fonksiyonu Bağla
    if (WaitTask)
    {
        WaitTask->EventReceived.AddDynamic(this, &UGA_HeroDashWithAnim::OnPerfectDodgeReceived);
        WaitTask->ReadyForActivation();
    }

    UGameplayEffect* GE_SpeedBoost = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(GE_GiveDashTag);
    GE_GiveDashTagHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(GE_SpeedBoost, 1, FGameplayEffectContextHandle());
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
    Super::ActivateAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);
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

void UGA_HeroDashWithAnim::OnPerfectDodgeReceived(FGameplayEventData Payload)
{
    UE_LOG(LogTemp, Warning, TEXT("Perfect Dodgeee!!!"));
}

void UGA_HeroDashWithAnim::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnEventReceived(EventTag, EventData);

    // During dash the character is allowed to rotate freely for a brief moment,
    // but once this notify triggers, rotation to movement is disabled to 
    // ensure consistent forward dash behavior.
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

    if (GE_GiveDashTagHandle.IsValid())
    {
        GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(GE_GiveDashTagHandle);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

