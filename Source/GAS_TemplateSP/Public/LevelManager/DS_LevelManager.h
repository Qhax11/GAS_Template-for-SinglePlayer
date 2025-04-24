// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "DS_LevelManager.generated.h"


UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "LevelManager"))
class GAS_TEMPLATESP_API UDS_LevelManager : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// Map name to widget class
	UPROPERTY(EditDefaultsOnly, Config, Category = "UI")
	TMap<FName, TSubclassOf<UUserWidget>> LevelToWidgetMap;

};
