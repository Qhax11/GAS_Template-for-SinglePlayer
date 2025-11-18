// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Hero/Router/GA_HeroRouterDashAndRun.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"


void UGA_HeroRouterDashAndRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UsedDashAbilty = GetASC()->TryActivateAbilityByClassAndReturnInstance(GA_DashAbilityClass);
    if(!UsedDashAbilty)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash Ability could not be activated in: %s"), *GetName());
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
	}

    UsedDashAbilty->OnAbilityEnded.RemoveAll(this);
    UsedDashAbilty->OnAbilityEnded.AddUObject(this, &UGA_HeroRouterDashAndRun::OnDashAbilityEnded);

    WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
    if (WaitRelease)
    {
        WaitRelease->OnRelease.AddDynamic(this, &UGA_HeroRouterDashAndRun::OnInputReleased);
        WaitRelease->ReadyForActivation();
    }

    InputPressedTime = GetWorld()->GetTimeSeconds();
}

void UGA_HeroRouterDashAndRun::OnDashAbilityEnded(const FCustomAbilityEndedData& DodgeAbilityEndedData)
{
    if (DodgeAbilityEndedData.bWasCancelled)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float HoldDuration = CurrentTime - InputPressedTime;

    if (HoldDuration >= MinInputHoldTimeForRun)
    {
        TryActivateRun();
    }
    else
    {
        float RemainingTime = MinInputHoldTimeForRun - HoldDuration;
        FTimerHandle TimerDel;
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimer(
                TimerDel,
                this,
                &UGA_HeroRouterDashAndRun::TryActivateRun,
                RemainingTime,
                false
            );
        }
    }
}

void UGA_HeroRouterDashAndRun::TryActivateRun()
{
    UsedRunAbilty = GetASC()->TryActivateAbilityByClassAndReturnInstance(GA_RunAbilityClass);
    if (!UsedRunAbilty)
    {
        UE_LOG(LogTemp, Warning, TEXT("Run Ability could not be activated after Dash in: %s"), *GetName());
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
    }
}

void UGA_HeroRouterDashAndRun::OnInputReleased(float TimeHeld)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_HeroRouterDashAndRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (WaitRelease && IsValid(WaitRelease))
    {
        WaitRelease->EndTask();
        WaitRelease = nullptr;
    }

    if (IsValid(UsedRunAbilty))
    {
        GetASC()->CancelAbilityHandle(UsedRunAbilty->GetCurrentAbilitySpecHandle());
        UsedRunAbilty = nullptr;
    }

    if (IsValid(UsedDashAbilty))
    {
        UsedDashAbilty->OnAbilityEnded.RemoveAll(this);
        UsedDashAbilty = nullptr;
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
