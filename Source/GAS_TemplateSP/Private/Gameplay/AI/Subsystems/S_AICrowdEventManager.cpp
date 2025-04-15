// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
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
        SpawnDelegatesSubsystem->OnEnemyDeSpawn.AddDynamic(this, &US_AICrowdEventManager::OnEnemyDeSpawn);
    }
 
    const UDS_AICrowdEventManager* AICrowdEventManagerSettings = GetDefault<UDS_AICrowdEventManager>();
    if (!AICrowdEventManagerSettings)
    {
        UE_LOG(LogTemp, Warning, TEXT("AICrowdEventManager Settings is null in %s"), *this->GetName());
        return;
    }

    FTimerHandle DebugTimerHandle;
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            DebugTimerHandle,
            this,
            &US_AICrowdEventManager::DebugPrintState,
            0.01f,
            true
        );
    }

    MaxEnemyAttackingCount = AICrowdEventManagerSettings->MaxEnemyAttackingCount;
    bDebug = AICrowdEventManagerSettings->bDebug;
}

bool US_AICrowdEventManager::RequestToBeAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!ASC)
    {
        return false;
    }

    if (ASC->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender))
    {
        return true; 
    }

    if (AttackIntenders.Contains(ASC))
    {
        return false;
    } 
    
    if (AttackIntenders.Num() >= MaxEnemyAttackingCount)
    {
        return false;
    }

    AddAttackIntender(ASC);

    return true;
}

void US_AICrowdEventManager::OnNewAttackIntenderAdded(UAbilitySystemComponent* NewIntender)
{
    if (!NewIntender)
    {
        return;
    }

    // It is already AttackIntender
    if (NewIntender->HasMatchingGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender))
    {
        return;
    }

    AddAttackIntender(NewIntender);

    if (AttackIntenders.Num() >= MaxEnemyAttackingCount) 
    {
        UAbilitySystemComponent* FurthestIntenderASC = GetFurthestAttackIntender(NewIntender);
        if (FurthestIntenderASC)
        {
            RemoveAttackIntender(FurthestIntenderASC);
            if (bDebug)
            {
                UE_LOG(LogTemp, Warning, TEXT("[Crowd] %s removed from AttackIntenders due to proximity override."),
                    *FurthestIntenderASC->GetAvatarActor()->GetName());
            }
        }
    }
}

bool US_AICrowdEventManager::ReleaseAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!ASC)
    {
        return false;
    }

    return RemoveAttackIntender(ASC);
}

void US_AICrowdEventManager::AddAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!ASC || AttackIntenders.Contains(ASC))
    {
        return;
    }

    NonAttackIntenders.Remove(ASC);
    AttackIntenders.Add(ASC);
    ASC->AddLooseGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender);
}

bool US_AICrowdEventManager::RemoveAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        UE_LOG(LogTemp, Warning, TEXT("ASC is invalid or being destroyed"));
        return false;
    }

    if (!AttackIntenders.Contains(ASC))
    {
        return false;
    }

    AttackIntenders.Remove(ASC);
    NonAttackIntenders.Add(ASC);
    ASC->RemoveLooseGameplayTag(GAS_Tags::TAG_AI_State_IsAttackIntender);
    return true;
}

UAbilitySystemComponent* US_AICrowdEventManager::GetFurthestAttackIntender(UAbilitySystemComponent* IgnoreASC) const
{
    if (!Hero) 
    {
        return nullptr;
    }

    UAbilitySystemComponent* FurthestASC = nullptr;
    float FurthestDistance = 0.f;

    for (UAbilitySystemComponent* ASC : AttackIntenders)
    {
        if (!ASC || ASC == IgnoreASC)
        {
            continue;
        }

        float Distance = FVector::Dist(ASC->GetAvatarActor()->GetActorLocation(), Hero->GetActorLocation());
        if (Distance > FurthestDistance)
        {
            FurthestDistance = Distance;
            FurthestASC = ASC;
        }
    }

    return FurthestASC;
}

UAbilitySystemComponent* US_AICrowdEventManager::GetClosestNonAttackIntender(UAbilitySystemComponent* IgnoreASC) const
{
    if (!Hero || AllEnemies.Num() == 0)
    {
        return nullptr;
    }

    UAbilitySystemComponent* ClosestEnemyASC = nullptr;
    float ClosestDistanceSqr = TNumericLimits<float>::Max();

    const FVector HeroLocation = Hero->GetActorLocation();

    for (UAbilitySystemComponent* EnemyASC : NonAttackIntenders)
    {
        if (!EnemyASC || EnemyASC == IgnoreASC)
        {
            continue;
        }

        const float DistanceSqr = FVector::DistSquared(HeroLocation, EnemyASC->GetAvatarActor()->GetActorLocation());

        if (DistanceSqr < ClosestDistanceSqr)
        {
            ClosestDistanceSqr = DistanceSqr;
            ClosestEnemyASC = EnemyASC;
        }
    }

    return ClosestEnemyASC;
}

void US_AICrowdEventManager::OnHeroSpawn(AGAS_CharacterBase* CharacterBase)
{
    if (!CharacterBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
        return;
    }

    Hero = CharacterBase;
}

void US_AICrowdEventManager::OnEnemySpawn(AGAS_CharacterBase* CharacterBase)
{
    if (!CharacterBase) 
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
        return;
    }

    UAbilitySystemComponent* EnemyASC = CharacterBase->GetAbilitySystemComponent();
    if (!EnemyASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyASC is null in: %s"), *GetName());
        return;
    }

    if (EnemyASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Boss)) 
    {
        return;
    }

    AllEnemies.Add(EnemyASC);
}

void US_AICrowdEventManager::OnEnemyDeSpawn(AGAS_CharacterBase* CharacterBase)
{
    if (!CharacterBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
        return;
    }

    AllEnemies.Remove(CharacterBase->GetAbilitySystemComponent());

    if (ReleaseAttackIntender(CharacterBase->GetAbilitySystemComponent())) 
    {
        UAbilitySystemComponent* ClosestNonAttackIntender = GetClosestNonAttackIntender(CharacterBase->GetAbilitySystemComponent());
        if (!ClosestNonAttackIntender)
        {
            return;
        }
        OnNewAttackIntenderAdded(ClosestNonAttackIntender);
    }
}

void US_AICrowdEventManager::DebugPrintState()
{
#if WITH_EDITOR
    if (!bDebug) 
    {
        return;
    }

    for (UAbilitySystemComponent* ASC : AllEnemies)
    {
        FString StatusText;
        if (AttackIntenders.Contains(ASC))
        {
            StatusText = TEXT("ATTACKING");
        }
        else if (NonAttackIntenders.Contains(ASC))
        {
            StatusText = TEXT("STRAFING");
        }
        else
        {
            StatusText = TEXT("UNKNOWN");
        }

        FVector TextLocation = ASC->GetAvatarActor()->GetActorLocation() + FVector(0.f, 0.f, 150.f);

        DrawDebugString(
            GetWorld(),
            TextLocation,
            StatusText,
            nullptr,
            StatusText == TEXT("ATTACKING") ? FColor::Red : FColor::Green,
            0.01f, 
            true
        );
    }
#endif // WITH_EDITOR

}



