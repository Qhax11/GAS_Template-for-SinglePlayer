// Qhax's GAS Template for SinglePlayer


#include "SoundManager/S_SoundManager.h"
#include "SoundManager/DS_SoundManager.h"
#include "LevelManager/S_LevelManager.h"
#include <Kismet/GameplayStatics.h>

void US_SoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(US_LevelManager::StaticClass());

	SoundManagerSettings = GetDefault<UDS_SoundManager>();
	if (!SoundManagerSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundManagerSettings is null in: %s"), *GetName());
		return;
	}

	LevelManager = GetGameInstance()->GetSubsystem<US_LevelManager>();
	if (!LevelManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManager is null in: %s"), *GetName());
		return;
	}

	// Play initial music with slight delay
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &US_SoundManager::FirstInit, 0.1f, false);

	LevelManager->OnLevelChanged.AddDynamic(this, &US_SoundManager::OnLevelChanged);
}

void US_SoundManager::FirstInit()
{
	PlayMusicToLevel(LevelManager->GetCleanLevelName());
}

void US_SoundManager::OnLevelChanged(FName LevelName)
{
	PlayMusicToLevel(LevelName);
}

void US_SoundManager::PlayMusicToLevel(FName LevelName)
{
	if (!SoundManagerSettings || !LevelManager)
	{
		UE_LOG(LogTemp, Error, TEXT("SoundManagerSettings or LevelManager is null!"));
		return;
	}

	if (const TSoftObjectPtr<USoundBase>* LevelMusicPtr = SoundManagerSettings->LevelToMusicMap.Find(LevelName))
	{
		if (LevelMusicPtr && LevelMusicPtr->IsValid())
		{
			UGameplayStatics::PlaySound2D(GetWorld(), LevelMusicPtr->Get());
			UE_LOG(LogTemp, Warning, TEXT("Playing preloaded music for level: %s"), *LevelName.ToString());
		}
		else
		{
			USoundBase* LoadedMusic = LevelMusicPtr->LoadSynchronous();
			if (LoadedMusic)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), LoadedMusic);
				UE_LOG(LogTemp, Warning, TEXT("Playing newly loaded music for level: %s"), *LevelName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load music for level: %s"), *LevelName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No music found for level: %s"), *LevelName.ToString());
	}
}
