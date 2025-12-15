// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroAbilityBuffer.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMeleeComboManager.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"

UAC_HeroAbilityBuffer::UAC_HeroAbilityBuffer()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_HeroAbilityBuffer::BeginPlay()
{
    Super::BeginPlay();

    if (!HeroBase || !HeroASC || !HeroTagDelegatesComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeroBase, HeroASC or HeroTagDelegatesComp is null in: %s)"), *GetName());
        return;
    }

    HeroMeleeComboManager = HeroBase->GetHeroMeleeComboManagerComponent();
    checkf(HeroMeleeComboManager, TEXT("HeroMeleeComboManager is null in %s"), *GetClass()->GetName());

    if (!HeroASC->AbilityFailedCallbacks.IsBoundToObject(this))
    {
        HeroASC->AbilityFailedCallbacks.AddUObject(this, &UAC_HeroAbilityBuffer::OnAbilityFailed);
    }

    HeroTagDelegatesComp->RegisterDelegateForTag(GAS_Tags::TAG_Gameplay_State_Phase_Active, EListenMode::OnRemoved).BindDynamic(this, &UAC_HeroAbilityBuffer::OnHeroPhaseActiveTagRemoved);
}

void UAC_HeroAbilityBuffer::OnAbilityFailed(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& TagExplaining)
{
    if (!FailedAbility) 
    {
        UE_LOG(LogTemp, Warning, TEXT("FailedAbility null in: %s"), *GetName());
        return;
    }

    // Only buffer during relevant phases
    if (!TagExplaining.HasTag(GAS_Tags::TAG_Gameplay_State_Phase_Active))
    {
        return;
    }

    // Save ability
    BufferedAbilityClass = FailedAbility->GetClass();

    // Reset timeout
    GetWorld()->GetTimerManager().ClearTimer(BufferTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        BufferTimerHandle,
        this,
        &UAC_HeroAbilityBuffer::ClearBuffer,
        BufferLifetime,
        false
    );
}

void UAC_HeroAbilityBuffer::OnHeroPhaseActiveTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    TryActivateBufferedAbility();
}

void UAC_HeroAbilityBuffer::TryActivateBufferedAbility()
{
    if (!BufferedAbilityClass || !HeroASC)
    {
        return;
    }

    TSubclassOf<UGameplayAbility> AbilityToActivate = BufferedAbilityClass;
    if (AbilityToActivate->IsChildOf(UGA_ComboMeleeAttack::StaticClass()))
    {
        if (HeroMeleeComboManager) 
        {
            HeroMeleeComboManager->ActivateComboMeleeAttackAbility();
            return;
        }
    }

    // Its mean tag is removed before end of ability.
    if (HeroASC->IsAbilityClassActive(AbilityToActivate)) 
    {
        UGAS_GameplayAbilityBase* ActiveAbility = HeroASC->GetActiveAbilityInstanceByClass(AbilityToActivate);
        if (ActiveAbility) 
        {
            ActiveAbility->OnAbilityEnded.Clear();
            ActiveAbility->OnAbilityEnded.AddUObject(this, &UAC_HeroAbilityBuffer::OnBufferAbilityEnd);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UAC_HeroAbilityBuffer: TryActivateAbilityByClass: %s"), *AbilityToActivate->GetName());
        HeroASC->TryActivateAbilityByClass(AbilityToActivate);
        ClearBuffer();
    }
}

void UAC_HeroAbilityBuffer::OnBufferAbilityEnd(const FCustomAbilityEndedData& ComboAbilityEndedData)
{
    HeroASC->TryActivateAbilityByClass(BufferedAbilityClass);
    ClearBuffer();
}

void UAC_HeroAbilityBuffer::ClearBuffer()
{
    BufferedAbilityClass = nullptr;
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(BufferTimerHandle);
    }
}

void UAC_HeroAbilityBuffer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    ClearBuffer(); 

    if (HeroASC)
    {
        HeroASC->AbilityFailedCallbacks.RemoveAll(this);
    }

    if (HeroTagDelegatesComp)
    {
        HeroTagDelegatesComp->UnregisterAllDelegatesForObject(this);
    }
}

