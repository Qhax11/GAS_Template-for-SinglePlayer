// Qhax's GAS Template for SinglePlayer


#include "LevelManager/S_LevelManager.h"
#include "LevelManager/DS_LevelManager.h"
#include "Kismet/GameplayStatics.h"


void US_LevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LevelManagerSettings = GetDefault<UDS_LevelManager>();
	if (!LevelManagerSettings) 
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSettings is null in: %s"), *GetName());
		return;
	}
	
	// Bind to world load complete
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &US_LevelManager::HandlePostLoadMap);
}

void US_LevelManager::Deinitialize()
{
	Super::Deinitialize();

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void US_LevelManager::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!LoadedWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("Loaded world is null!"));
		return;
	}

	FName CleanLevelName = GetCleanLevelName();
	OnLevelChanged.Broadcast(CleanLevelName);

	UE_LOG(LogTemp, Warning, TEXT("Broadcasted loaded level: %s"), *CleanLevelName.ToString());
}

void US_LevelManager::OpenLevelByName(FName LevelName)
{
	if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		UE_LOG(LogTemp, Log, TEXT("[LevelManagerSubsystem] Opening level: %s"), *LevelName.ToString());

		CurrentLevelName = LevelName;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LevelManagerSubsystem] Tried to open level with invalid name."));
	}
}

bool US_LevelManager::IsCurrentLevel(FName LevelName) const
{
	return GetCleanLevelName() == LevelName.ToString();
}

FName US_LevelManager::GetCleanLevelName() const
{
	if (!GetWorld())
	{
		return FName();
	}

	FString RawLevelName = GetWorld()->GetMapName();
	FString CleanLevelName;

	// When running in editor, level names have a "UEDPIE_" prefix
	if (RawLevelName.StartsWith(TEXT("UEDPIE_")))
	{
		// Split from the end to retrieve the actual level name
		RawLevelName.Split(TEXT("_"), nullptr, &CleanLevelName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	}
	else
	{
		CleanLevelName = RawLevelName;
	}

	return FName(CleanLevelName);
}

