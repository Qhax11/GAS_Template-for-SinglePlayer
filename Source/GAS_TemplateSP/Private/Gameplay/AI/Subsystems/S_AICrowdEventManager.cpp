// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/Subsystems/S_AICrowdEventManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/AI/DS_AICrowdEventManager.h"
#include "AIController.h"

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

    FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &US_AICrowdEventManager::OnWorldBeginTearDown);

    MaxEnemyAttackingCount = AICrowdEventManagerSettings->MaxEnemyAttackingCount;
    bDebug = AICrowdEventManagerSettings->bDebug;

#if WITH_EDITOR
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
#endif // WITH_EDITOR
}

void US_AICrowdEventManager::OnWorldBeginTearDown(UWorld* World)
{
    Enemies.Empty();
    HeroActor = nullptr;

    UE_LOG(LogTemp, Warning, TEXT("[CrowdManager] OnWorldBeginTearDown: Enemies cleared, HeroActor nulled."));
}

void US_AICrowdEventManager::OnHeroSpawn(const FHeroSpawnData& HeroSpawnData)
{
    if (!HeroSpawnData.Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is null in: %s"), *GetName());
        return;
    }

    HeroActor = HeroSpawnData.Character;
}

void US_AICrowdEventManager::OnEnemySpawn(const FEnemySpawnData& EnemySpawnData)
{
    if (!EnemySpawnData.Character || !EnemySpawnData.ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character or ASC null in: %s"), *GetName());
        return;
    }

    FEnemyData EnemeyData = FEnemyData(EnemySpawnData.Character, EnemySpawnData.ASC, EnemySpawnData.StateTree);
    Enemies.Add(EnemeyData);
}

void US_AICrowdEventManager::OnEnemyDeSpawn(const FCharacterDeSpawnData& CharacterDeSpawnData)
{
    if (!CharacterDeSpawnData.Character || !CharacterDeSpawnData.ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character or ASC null in: %s"), *GetName());
        return;
    }

    SafeRemoveEnemyByASC(CharacterDeSpawnData.ASC);

    FEnemyData* ClosestNonAttackIntenderData = GetClosestNonAttackIntender(CharacterDeSpawnData.ASC);
    if (!ClosestNonAttackIntenderData)
    {
        return;
    }
    OnNewAttackIntenderAdded(ClosestNonAttackIntenderData->ASC);

    CharacterDeSpawnData.InstigatorASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_State_InCombat_Finisher);
    OnRequestEnemyBackupReaction.Broadcast();
}

bool US_AICrowdEventManager::RequestToBeAttackIntender(UAbilitySystemComponent* ASC)
{
    if (!IsValid(ASC))
    {
        return false;
    }

    FEnemyData* FindedEnemyData = FindEnemyDataByASC(ASC);
    if (!FindedEnemyData)
    {
        return false;
    }

    // It is already AttackIntender
    if (FindedEnemyData->IsAttackIntender())
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
    //ForceAddAttackIntender(NewIntender);
}

bool US_AICrowdEventManager::ForceAddAttackIntender(UAbilitySystemComponent* NewIntender)
{
    if (!IsValid(NewIntender))
    {
        return false;
    }

    FEnemyData* NewEnemyData = FindEnemyDataByASC(NewIntender);
    if (!NewEnemyData)
    {
        return false;
    }

    // Already an attack intender
    if (NewEnemyData->IsAttackIntender())
    {
        return false;
    }

    // Check if we need to steal token
    const int32 CurrentIntenders = GetAttackIntenders().Num();
    if (CurrentIntenders >= MaxEnemyAttackingCount)
    {
        FEnemyData* ClosestIntender = GetClosestAttackIntender(NewIntender);
        if (ClosestIntender && ClosestIntender->ASC != NewIntender)
        {
            RemoveAttackIntender(ClosestIntender->ASC);

            if (bDebug && ClosestIntender->Character)
            {
                UE_LOG(LogTemp, Warning, TEXT("[Crowd] %s lost attack token to %s."),
                    *ClosestIntender->ASC->GetName(), *NewIntender->GetName());
            }
        }
        else
        {
            // No one to remove; cannot add
            return false;
        }
    }

    // Add new intender
    AddAttackIntender(NewIntender);

    if (bDebug && NewEnemyData->Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Crowd] %s added as AttackIntender."), *NewIntender->GetName());
    }

    return true;
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

TArray<FEnemyData*> US_AICrowdEventManager::GetAttackIntenders() 
{
    TArray<FEnemyData*> Result;
    for (FEnemyData& Data : Enemies)
    {
        if (Data.IsAttackIntender())
        {
            Result.Add(&Data);
        }
    }
    return Result;
}

TArray<FEnemyData*> US_AICrowdEventManager::GetNonAttackIntenders() 
{
    TArray<FEnemyData*> Result;
    for (FEnemyData& Data : Enemies)
    {
        if (!Data.IsAttackIntender())
        {
            Result.Add(&Data);
        }
    }
    return Result;
}

FEnemyData* US_AICrowdEventManager::GetFurthestAttackIntender(UAbilitySystemComponent* IgnoreASC) 
{
    if (!HeroActor)
    {
        return nullptr;
    }

    FEnemyData* FurthestEnemyData = nullptr;
    float FurthestDistance = 0.f;

    for (FEnemyData* Data : GetAttackIntenders())
    {
        if (!Data || !Data->Character || Data->ASC == IgnoreASC)
        {
            continue;
        }

        float Distance = FVector::Dist(Data->Character->GetActorLocation(), HeroActor->GetActorLocation());
        if (Distance > FurthestDistance)
        {
            FurthestDistance = Distance;
            FurthestEnemyData = Data;
        }
    }

    return FurthestEnemyData;
}

FEnemyData* US_AICrowdEventManager::GetClosestAttackIntender(UAbilitySystemComponent* IgnoreASC)
{
    if (!HeroActor || GetAttackIntenders().Num() == 0)
    {
        return nullptr;
    }

    FEnemyData* ClosestEnemyData = nullptr;
    float ClosestDistanceSqr = TNumericLimits<float>::Max();

    const FVector HeroLocation = HeroActor->GetActorLocation();

    for (FEnemyData* Data : GetAttackIntenders())
    {
        if (!Data || !Data->Character || Data->ASC == IgnoreASC)
        {
            continue;
        }

        const float DistanceSqr = FVector::DistSquared(HeroLocation, Data->Character->GetActorLocation());

        if (DistanceSqr < ClosestDistanceSqr)
        {
            ClosestDistanceSqr = DistanceSqr;
            ClosestEnemyData = Data;
        }
    }

    return ClosestEnemyData;
}

FEnemyData* US_AICrowdEventManager::GetClosestNonAttackIntender(UAbilitySystemComponent* IgnoreASC) 
{
    if (!HeroActor || GetNonAttackIntenders().Num() == 0)
    {
        return nullptr;
    }

    FEnemyData* ClosestEnemyData = nullptr;
    float ClosestDistanceSqr = TNumericLimits<float>::Max();

    const FVector HeroLocation = HeroActor->GetActorLocation();

    for (FEnemyData* Data : GetNonAttackIntenders())
    {
        if (!Data || !Data->Character || Data->ASC == IgnoreASC)
        {
            continue;
        }

        const float DistanceSqr = FVector::DistSquared(HeroLocation, Data->Character->GetActorLocation());

        if (DistanceSqr < ClosestDistanceSqr)
        {
            ClosestDistanceSqr = DistanceSqr;
            ClosestEnemyData = Data;
        }
    }

    return ClosestEnemyData;
}

#if WITH_EDITOR
void US_AICrowdEventManager::DebugPrintState()
{
    if (!bDebug) 
    {
        return;
    }

    for (FEnemyData EnemyData : Enemies)
    {
        if (!EnemyData.Character) 
        {
            continue;
        }

        FString StatusText;
        if (EnemyData.IsAttackIntender())
        {
            StatusText = TEXT("ATTACKING");
        }
        else 
        {
            StatusText = TEXT("STRAFING");
        }
       
        FVector TextLocation = EnemyData.Character->GetActorLocation() + FVector(0.f, 0.f, 150.f);

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
}
#endif // WITH_EDITOR





