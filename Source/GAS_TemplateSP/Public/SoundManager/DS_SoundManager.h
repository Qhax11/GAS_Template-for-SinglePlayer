// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Sound/SoundBase.h"
#include "DS_SoundManager.generated.h"


UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Project Systems | Subsystems | Sound Manager"))
class GAS_TEMPLATESP_API UDS_SoundManager : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, Category = "Sound Manager")
	TMap<FName, TSoftObjectPtr<USoundBase>> LevelToMusicMap;
};
