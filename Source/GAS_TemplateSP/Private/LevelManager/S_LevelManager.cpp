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

void US_LevelManager::OpenLevel(FName LevelName, bool bShowLoadingScreen)
{
	if (LevelName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[LevelManagerSubsystem] Tried to open level with invalid name."));
		return;
	}

	bShowStartupLoading = bShowLoadingScreen;
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	CurrentLevelName = LevelName;
}

bool US_LevelManager::IsCurrentLevel(FName LevelName) const
{
	return GetCleanLevelName() == LevelName.ToString();
}

FName US_LevelManager::GetCleanLevelName() const
{
	if (!GetWorld())
	{
		return TEXT("InvalidWorld");
	}

	// PIE veya Editor kopyasý da dahil olmak üzere GetMapName() al
	FString MapName = GetWorld()->GetMapName(); // UEDPIE_0_MedievalVillage_P_WP gibi

	// PIE prefixini kaldýr
	MapName.RemoveFromStart(TEXT("UEDPIE_0_"));

	// Path varsa sadece son kýsmý al
	int32 LastSlash;
	if (MapName.FindLastChar('/', LastSlash))
	{
		MapName = MapName.Mid(LastSlash + 1);
	}

	return FName(MapName);
}

