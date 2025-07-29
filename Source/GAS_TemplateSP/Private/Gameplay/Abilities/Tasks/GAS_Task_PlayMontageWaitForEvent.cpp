// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Tasks/GAS_Task_PlayMontageWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Runtime/Launch/Resources/Version.h"


void UGAS_Task_PlayMontageWaitForEvent::Activate()
{
    if (!Ability)
    {
        return;
    }

    bool bPlayedMontage = false;

    if (AbilitySystemComponent.IsValid())
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            // Bind to event callback
            EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGAS_Task_PlayMontageWaitForEvent::OnGameplayEvent));

            float CurrentMontageSectionTimeLeft = AbilitySystemComponent->GetCurrentMontageSectionTimeLeft();
            if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
            {
                // Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
                if (ShouldBroadcastAbilityTaskDelegates() == false)
                {
                    return;
                }

                CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UGAS_Task_PlayMontageWaitForEvent::OnAbilityCancelled);

                BlendingOutDelegate.BindUObject(this, &UGAS_Task_PlayMontageWaitForEvent::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

                MontageEndedDelegate.BindUObject(this, &UGAS_Task_PlayMontageWaitForEvent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

                if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
                {
                    Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
                }

                bPlayedMontage = true;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UGAS_Task_PlayMontageWaitForEvent call to PlayMontage failed!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UGAS_Task_PlayMontageWaitForEvent called on invalid AbilitySystemComponent"));
    }

    if (!bPlayedMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("UGAS_Task_PlayMontageWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    SetWaitingOnAvatar();
}

void UGAS_Task_PlayMontageWaitForEvent::ExternalCancel()
{
    check(AbilitySystemComponent.IsValid());

    OnAbilityCancelled();
    Super::ExternalCancel();
}

FString UGAS_Task_PlayMontageWaitForEvent::GetDebugString() const
{
    const UAnimMontage* PlayingMontage = nullptr;
    if (Ability)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

        if (AnimInstance != nullptr)
        {
            PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay.Get() : AnimInstance->GetCurrentActiveMontage();
        }
    }

    return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UGAS_Task_PlayMontageWaitForEvent::OnDestroy(bool AbilityEnded)
{
    // Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
   // (If we are destroyed, it will detect this and not do anything)

   // This delegate, however, should be cleared as it is a multicast
    if (Ability)
    {
        Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
        if (AbilityEnded && bStopWhenAbilityEnds)
        {
            StopPlayingMontage();
        }
    }

    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
    }

    Super::OnDestroy(AbilityEnded);
}

void UGAS_Task_PlayMontageWaitForEvent::UnbindAllDelegate()
{
    OnCompleted.Clear();
    OnBlendOut.Clear();
    OnInterrupted.Clear();
}

UGAS_Task_PlayMontageWaitForEvent* UGAS_Task_PlayMontageWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
    UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

    if (!OwningAbility)
    {
        // Prevent crash if two abilities bound to input action.
        // TODO: Investigate
        UE_LOG(LogTemp, Error, TEXT("UGAS_Task_PlayMontageWaitForEvent - Owning Ability is null. This should never happen."));
        return nullptr;
    }

    UGAS_Task_PlayMontageWaitForEvent* MyObj = NewAbilityTask<UGAS_Task_PlayMontageWaitForEvent>(OwningAbility, TaskInstanceName);
    MyObj->MontageToPlay = MontageToPlay;
    MyObj->EventTags = EventTags;
    MyObj->Rate = Rate;
    MyObj->StartSection = StartSection;
    MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
    MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

    return MyObj;
}

bool UGAS_Task_PlayMontageWaitForEvent::StopPlayingMontage() const
{
    if (!AbilitySystemComponent.IsValid() || !Ability)
    {
        return false;
    }

    const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
    if (!ActorInfo)
    {
        return false;
    }

    const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
    if (AnimInstance == nullptr)
    {
        return false;
    }

    // Check if the montage is still playing
    // The ability would have been interrupted, in which case we should automatically stop the montage
    if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
    {
        // Unbind delegates so they don't get called as well
        FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
        if (MontageInstance)
        {
            MontageInstance->OnMontageBlendingOutStarted.Unbind();
            MontageInstance->OnMontageEnded.Unbind();
        }

        AbilitySystemComponent->CurrentMontageStop();
        return true;
    }

    return false;
}

void UGAS_Task_PlayMontageWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted) const
{
    if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
    {
        if (Montage == MontageToPlay)
        {
            AbilitySystemComponent->ClearAnimatingAbility(Ability);

            // Reset AnimRootMotionTranslationScale
            ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
            if (Character && (Character->GetLocalRole() == ROLE_Authority || (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
            {
                Character->SetAnimRootMotionTranslationScale(1.f);
            }

        }
    }

    if (bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
    else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UGAS_Task_PlayMontageWaitForEvent::OnAbilityCancelled() const
{
    if (StopPlayingMontage())
    {
        // Let the BP handle the interrupt as well
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UGAS_Task_PlayMontageWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    EndTask();
}

void UGAS_Task_PlayMontageWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const
{
    if (!ShouldBroadcastAbilityTaskDelegates())
    {
        return;
    }

    FGameplayEventData TempData = *Payload;
    TempData.EventTag = EventTag;

    EventReceived.Broadcast(EventTag, TempData);
}

