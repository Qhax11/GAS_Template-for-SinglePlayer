// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "UI/S_UIManager.h"
#include "DS_LevelManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	EUIWidgetContext Context = EUIWidgetContext::NonGameplay;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "LevelManager"))
class GAS_TEMPLATESP_API UDS_LevelManager : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// Map name to widget class
	UPROPERTY(EditDefaultsOnly, Config, Category = "UI")
	TMap<FName, FLevelWidgetData> LevelToWidgetMap;

};
