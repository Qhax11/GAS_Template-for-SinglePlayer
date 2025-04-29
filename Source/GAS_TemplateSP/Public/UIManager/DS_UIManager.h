// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "DS_UIManager.generated.h"

UENUM(BlueprintType)
enum class EUIWidgetContext : uint8
{
	GameOnly,     // Only gameplay inputs are enabled (movement, attacks, etc.)
	UIOnly,       // Only UI inputs are enabled (menus, settings, etc.)
	GameAndUI,    // Both gameplay and UI inputs are enabled
};

UENUM(BlueprintType)
enum class EGamePauseBehavior : uint8
{
	NoPause,   
	PauseGame  
};

USTRUCT(BlueprintType)
struct FWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	EUIWidgetContext Context = EUIWidgetContext::GameOnly;

	UPROPERTY(EditAnywhere)
	EGamePauseBehavior PauseBehavior = EGamePauseBehavior::NoPause;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Project Systems | Subsystems | UI | UI Manager"))
class GAS_TEMPLATESP_API UDS_UIManager : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// Map name to widget class
	UPROPERTY(EditDefaultsOnly, Config, Category = "UI")
	TMap<FName, FWidgetData> LevelToWidgetMap;

	UPROPERTY(Config, EditDefaultsOnly, Category = "UI")
	FWidgetData ToggleMenu;

	UPROPERTY(Config, EditDefaultsOnly, Category = "UI")
	FWidgetData SettingsMenu;
	
};
