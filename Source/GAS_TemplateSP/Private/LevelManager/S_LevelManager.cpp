// Qhax's GAS Template for SinglePlayer


#include "LevelManager/S_LevelManager.h"
#include "LevelManager/DS_LevelManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Kismet/GameplayStatics.h"

struct FLevelWidgetData;

void US_LevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_SpawnDelegates::StaticClass());

	LevelManagerSettings = GetDefault<UDS_LevelManager>();
	if (!LevelManagerSettings) 
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSettings is null in: %s"), *GetName());
		return;
	}

	US_SpawnDelegates* SpawnDelegatesSubsystem = GetGameInstance()->GetSubsystem<US_SpawnDelegates>();
	if (!SpawnDelegatesSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnDelegatesSubsystem is null in: %s"), *GetName());
		return;
	}

	SpawnDelegatesSubsystem->OnPlayerControllerSpawn.AddDynamic(this, &US_LevelManager::OnPlayerControllerSpawn);
}

void US_LevelManager::OnPlayerControllerSpawn(APlayerController* PC)
{
	if (!GetWorld())
	{
		return;
	}

	FString CleanLevelName = GetCleanLevelName();

	CurrentLevelName = FName(*CleanLevelName);
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

FString US_LevelManager::GetCleanLevelName() const
{
	if (!GetWorld())
	{
		return FString();
	}

	FString RawLevelName = GetWorld()->GetMapName();
	FString CleanLevelName;

	// Eðer editorda çalýþýyorsan baþýnda UEDPIE_ gibi prefix oluyor
	if (RawLevelName.StartsWith(TEXT("UEDPIE_")))
	{
		// Sondan split yaparak asýl level adýný alýyoruz
		RawLevelName.Split(TEXT("_"), nullptr, &CleanLevelName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	}
	else
	{
		CleanLevelName = RawLevelName;
	}

	return CleanLevelName;
}

