// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Tutorial/S_TutorialManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"


void US_TutorialManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroSpawn.AddDynamic(this, &US_TutorialManager::OnHeroSpawn);
    }
}

void US_TutorialManager::OnHeroSpawn(const FHeroSpawnData& HeroSpawnData)
{
    if (!HeroSpawnData.Character) 
    {
        return;
    }

    Hero = Cast<AGAS_HeroBase>(HeroSpawnData.Character);
    if (!Hero) 
    {
        return;
    }

    if (UAC_TargetLockSystem* HeroTargetLockSystemComp = Hero->GetTargetLockSystemComponent()) 
    {
        HeroTargetLockSystemComp->OnTargetChanged.AddDynamic(this, &US_TutorialManager::OnTargetChanged);
    }

    UAC_TagDelegates* HeroTagDelegatesComp = HeroSpawnData.Character->GetTagDelegatesComponent();
    if (!HeroTagDelegatesComp)
    { 
        return;
    }

    HeroTagDelegatesComp->RegisterDelegateForTag(
        GAS_Tags::TAG_Gameplay_State_InCombat_ParryKnockback, EListenMode::OnAdded).BindDynamic(this, &US_TutorialManager::OnParryKnockbackTagAdded);
    

}

void US_TutorialManager::OnParryKnockbackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    UE_LOG(LogTemp, Warning, TEXT("HERO MADE KNOCKBACK!"));
}

void US_TutorialManager::OnTargetChanged(AActor* NewTarget)
{
    UE_LOG(LogTemp, Warning, TEXT("HERO TARGET LOCKED!"));
}


