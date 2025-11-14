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

    if (!UsedDashAbilty->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UGA_HeroRouterDashAndRun::OnDashAbilityEnded)) 
    {
        UsedDashAbilty->OnGameplayAbilityEndedWithDataBP.AddDynamic(this, &UGA_HeroRouterDashAndRun::OnDashAbilityEnded);
    }

    WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
    if (WaitRelease)
    {
        WaitRelease->OnRelease.AddDynamic(this, &UGA_HeroRouterDashAndRun::OnInputReleased);
        WaitRelease->ReadyForActivation();
    }
}

void UGA_HeroRouterDashAndRun::OnDashAbilityEnded(const FAbilityEndedDataBP& DodgeAbilityEndedData)
{
    UsedRunAbilty = GetASC()->TryActivateAbilityByClassAndReturnInstance(GA_RunAbilityClass);
}

void UGA_HeroRouterDashAndRun::OnInputReleased(float TimeHeld)
{
    if (WaitRelease && IsValid(WaitRelease))
    {
        WaitRelease->EndTask();
        WaitRelease = nullptr;
    }

    if (IsValid(UsedRunAbilty))
    {
        GetASC()->CancelAbilityHandle(UsedRunAbilty->GetCurrentAbilitySpecHandle());
        UsedDashAbilty = nullptr;
    }

    if (IsValid(UsedDashAbilty))
    {
        if (UsedDashAbilty->OnGameplayAbilityEndedWithDataBP.IsAlreadyBound(this, &UGA_HeroRouterDashAndRun::OnDashAbilityEnded))
        {
            UsedDashAbilty->OnGameplayAbilityEndedWithDataBP.RemoveDynamic(this, &UGA_HeroRouterDashAndRun::OnDashAbilityEnded);
        }
        UsedDashAbilty = nullptr;
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}