// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Gameplay/Components/AC_TagDelegates.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "Gameplay/AI/DS_AICrowdEventManager.h"


void US_AICrowdEventManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

    if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
    {
        SpawnDelegatesSubsystem->OnHeroSpawn.AddDynamic(this, &US_AICrowdEventManager::OnHeroSpawn);
        SpawnDelegatesSubsystem->OnEnemySpawn.AddDynamic(this, &US_AICrowdEventManager::OnEnemySpawn);
    }
 
    const UDS_AICrowdEventManager* AICrowdEventManagerSettings = GetDefault<UDS_AICrowdEventManager>();
    if (!AICrowdEventManagerSettings)
    {
        UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager Settings is null in %s"), *this->GetName());
        return;
    }

    MaxEnemyAttackingCount = AICrowdEventManagerSettings->MaxEnemyAttackingCount;
    bDebug = AICrowdEventManagerSettings->bDebug;
}

void US_AICrowdEventManager::OnHeroSpawn(AGAS_CharacterBase* CharacterBase)
{
    Hero = CharacterBase;
}

void US_AICrowdEventManager::OnEnemySpawn(AGAS_CharacterBase* CharacterBase)
{
    if (!CharacterBase) 
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
        return;
    }

    AAIController* EnemyController = Cast<AAIController>(CharacterBase->GetController());
    if (!EnemyController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is null in: %s"), *GetName());
        return;
    }

    UAbilitySystemComponent* EnemyASC = CharacterBase->GetAbilitySystemComponent();
    if (!EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyASC is null in: %s"), *GetName());
        return;
    }

    AllEnemies.Add(FEnemyData(EnemyASC, EnemyController));

    UAC_TagDelegates* TagDelegatesComponent = CharacterBase->GetTagDelegatesComponent();
    if (!TagDelegatesComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("TagDelegatesComponent is null in %s, cannot listen tags."), *this->GetName());
        return;
    }

    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_Attack, EListenMode::OnAdded).BindDynamic(this, &US_AICrowdEventManager::OnAttackTagAdded);
    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_Attack, EListenMode::OnRemoved).BindDynamic(this, &US_AICrowdEventManager::OnAttackTagRemoved);
    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_MovingToAttack, EListenMode::OnAdded).BindDynamic(this, &US_AICrowdEventManager::OnMoveToAttackTagAdded);
    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_MovingToAttack, EListenMode::OnRemoved).BindDynamic(this, &US_AICrowdEventManager::OnMoveToAttackTagRemoved);
    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_SoCloseToHero, EListenMode::OnAdded).BindDynamic(this, &US_AICrowdEventManager::OnSoCloseToHeroTagAdded);
    TagDelegatesComponent->RegisterDelegateForTag(GAS_Tags::TAG_AI_State_SoCloseToHero, EListenMode::OnRemoved).BindDynamic(this, &US_AICrowdEventManager::OnSoCloseToHeroTagAdded);
}

void US_AICrowdEventManager::OnAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    EnemyAttackingCount++;
    if (EnemyAttackingCount >= MaxEnemyAttackingCount)
    {
        SetValueToBlackboards(false);
        if (bDebug)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Values set to false!"));
        }
    }
}

void US_AICrowdEventManager::OnAttackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    EnemyAttackingCount--;
    if (EnemyAttackingCount < MaxEnemyAttackingCount)
    {
        SetValueToBlackboards(true);
        if (bDebug)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Values set to true!"));
        }
    }
}

void US_AICrowdEventManager::OnMoveToAttackTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
    UAbilitySystemComponent* NonConstASC = const_cast<UAbilitySystemComponent*>(AbilitySystemComponent);
}

void US_AICrowdEventManager::OnMoveToAttackTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{

}

void US_AICrowdEventManager::OnSoCloseToHeroTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}

void US_AICrowdEventManager::OnSoCloseToHeroTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag)
{
}

void US_AICrowdEventManager::SetValueToBlackboards(bool Value)
{
    for (FEnemyData& Data : AllEnemies)
    {
        UAbilitySystemComponent* EnemyASC = Data.EnemyASC;
        AAIController* EnemyController = Data.EnemyController;

        if (!EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_Attack))
        {
            EnemyController->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanAttack")), Value);
        }
    }
}


