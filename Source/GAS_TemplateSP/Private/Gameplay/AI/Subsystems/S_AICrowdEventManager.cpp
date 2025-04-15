// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
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

void US_AICrowdEventManager::OnHeroSpawn(const FCharacterSpawnData& CharacterSpawnData)
{
    if (!CharacterSpawnData.CharacterBase)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase is null in: %s"), *GetName());
        return;
    }

    Hero = CharacterSpawnData.CharacterBase;
}

void US_AICrowdEventManager::OnEnemySpawn(const FCharacterSpawnData& CharacterSpawnData)
{
    if (!CharacterSpawnData.CharacterBase || !CharacterSpawnData.ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase or ASC null in: %s"), *GetName());
        return;
    }

    if (CharacterSpawnData.ASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Boss))
    {
        return;
    }

    Enemies.Add(FEnemyData(CharacterSpawnData.ASC));
}

void US_AICrowdEventManager::OnEnemyDeSpawn(const FCharacterSpawnData& CharacterSpawnData)
{
    if (!CharacterSpawnData.CharacterBase || !CharacterSpawnData.ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterBase or ASC null in: %s"), *GetName());
        return;
    }

    SafeRemoveEnemyByASC(CharacterSpawnData.ASC);

    UAbilitySystemComponent* ClosestNonAttackIntender = GetClosestNonAttackIntender(CharacterSpawnData.ASC);
    if (!ClosestNonAttackIntender)
    {
        return;
    }
    OnNewAttackIntenderAdded(ClosestNonAttackIntender);
}

bool US_AICrowdEventManager::RequestToBeAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return false;
    }

    // It is already AttackIntender
    if (GetAttackIntenders().Contains(ASC))
    {
        return true;
    }

    if (GetAttackIntenders().Num() >= MaxEnemyAttackingCount)
    {
        return false;
    }

    return AddAttackIntender(ASC);
}

void US_AICrowdEventManager::OnNewAttackIntenderAdded(UAbilitySystemComponent* NewIntender)
{
    if (!IsValid(NewIntender))
    {
        return;
    }

    // It is already AttackIntender
    if (GetAttackIntenders().Contains(NewIntender))
    {
        return;
    }

    AddAttackIntender(NewIntender);

    if (GetAttackIntenders().Num() >= MaxEnemyAttackingCount)
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
    if (!IsValid(ASC))
    {
        return false;
    }

    return RemoveAttackIntender(ASC);
}

bool US_AICrowdEventManager::AddAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return false;
    }

    FEnemyData* FindedData = FindEnemyDataByASC(ASC);
    if (!FindedData)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASC couldn't find in Enemies: %s"), *GetNameSafe(ASC));
        return false;
    }

    FindedData->SetAttackIntender(true);
    return true;
}

bool US_AICrowdEventManager::RemoveAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return false;
    }

    FEnemyData* FindedData = FindEnemyDataByASC(ASC);
    if (!FindedData)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASC couldn't find in Enemies: %s"), *GetNameSafe(ASC));
        return false;
    }

    FindedData->SetAttackIntender(false);
    return true;
}

bool US_AICrowdEventManager::SafeRemoveEnemyByASC(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return false;
    }

    int32 Index = Enemies.IndexOfByPredicate([ASC](const FEnemyData& Data)
        {
            return Data.ASC == ASC;
        });

    if (Index == INDEX_NONE)
    {
        return false;
    }

    Enemies.RemoveAt(Index);
    return true;
}

FEnemyData* US_AICrowdEventManager::FindEnemyDataByASC(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return nullptr;
    }

    for (FEnemyData& Data : Enemies)
    {
        if (Data.ASC == ASC)
        {
            return &Data;
        }
    }

    return nullptr;
}

TArray<UAbilitySystemComponent*> US_AICrowdEventManager::GetAttackIntenders() const
{
    TArray<UAbilitySystemComponent*> Result;
    for (const FEnemyData& Data : Enemies)
    {
        if (Data.IsAttackIntender())
        {
            Result.Add(Data.ASC);
        }
    }
    return Result;
}

TArray<UAbilitySystemComponent*> US_AICrowdEventManager::GetNonAttackIntenders() const
{
    TArray<UAbilitySystemComponent*> Result;
    for (const FEnemyData& Data : Enemies)
    {
        if (!Data.IsAttackIntender())
        {
            Result.Add(Data.ASC);
        }
    }
    return Result;
}

UAbilitySystemComponent* US_AICrowdEventManager::GetFurthestAttackIntender(UAbilitySystemComponent* IgnoreASC) const
{
    if (!Hero) 
    {
        return nullptr;
    }

    UAbilitySystemComponent* FurthestASC = nullptr;
    float FurthestDistance = 0.f;

    for (UAbilitySystemComponent* ASC : GetAttackIntenders())
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
    if (!Hero || GetNonAttackIntenders().Num() == 0)
    {
        return nullptr;
    }

    UAbilitySystemComponent* ClosestEnemyASC = nullptr;
    float ClosestDistanceSqr = TNumericLimits<float>::Max();

    const FVector HeroLocation = Hero->GetActorLocation();

    for (UAbilitySystemComponent* EnemyASC : GetNonAttackIntenders())
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

void US_AICrowdEventManager::DebugPrintState()
{
#if WITH_EDITOR
    if (!bDebug) 
    {
        return;
    }

    for (FEnemyData EnemyData : Enemies)
    {
        FString StatusText;
        if (EnemyData.IsAttackIntender())
        {
            StatusText = TEXT("ATTACKING");
        }
        else 
        {
            StatusText = TEXT("STRAFING");
        }
       
        FVector TextLocation = EnemyData.ASC->GetAvatarActor()->GetActorLocation() + FVector(0.f, 0.f, 150.f);

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





