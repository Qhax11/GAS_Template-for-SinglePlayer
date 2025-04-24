// Qhax's GAS Template for SinglePlayer


#include "LevelManager/S_LevelManager.h"
#include "LevelManager/DS_LevelManager.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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

	if (US_SpawnDelegates* SpawnDelegatesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<US_SpawnDelegates>())
	{
		SpawnDelegatesSubsystem->OnPlayerControllerSpawn.AddDynamic(this, &US_LevelManager::OnPlayerControllerSpawn);
	}
}

void US_LevelManager::OnPlayerControllerSpawn(APlayerController* PC)
{
	if (!GetWorld())
	{
		return;
	}

	FString RawLevelName = GetWorld()->GetMapName(); // "UEDPIE_0_MainMenu" 
	FString CleanLevelName;

	if (RawLevelName.StartsWith(TEXT("UEDPIE_")))
	{
		RawLevelName.Split(TEXT("_"), nullptr, &CleanLevelName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	}
	else
	{
		CleanLevelName = RawLevelName;
	}

	if (LevelManagerSettings->LevelToWidgetMap.Contains(*CleanLevelName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found widget class for map!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO widget found for: %s"), *CleanLevelName);
	}

	if (const TSubclassOf<UUserWidget>* FoundWidget = LevelManagerSettings->LevelToWidgetMap.Find(*CleanLevelName))
	{
		CreateLevelWidget(PC, FoundWidget);
	}

	CurrentLevelName = FName(*CleanLevelName);
}

void US_LevelManager::CreateLevelWidget(APlayerController* PC, const TSubclassOf<UUserWidget>* WidgetClass)
{
	if (!WidgetClass)
	{
		return;
	}

	UUserWidget* CreatedWidget = nullptr;

	if (PC)
	{
		CreatedWidget = UWidgetBlueprintLibrary::Create(GetWorld(), *WidgetClass, PC);
		if (CreatedWidget) 
		{
			CreatedWidget->AddToViewport();
		}
	}
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

