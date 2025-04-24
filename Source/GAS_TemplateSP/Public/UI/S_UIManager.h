// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_UIManager.generated.h"

UENUM(BlueprintType)
enum class EUIWidgetContext : uint8
{
	Gameplay,       // HUD, Damage Popup, Combat Indicator, etc.
	NonGameplay,    // Main Menu, Settings, Tutorial, ESC, etc.
	PauseMenu,      // ESC menus that should pause game
};

UCLASS()
class GAS_TEMPLATESP_API US_UIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateAndShowWidget(TSubclassOf<UUserWidget> WidgetClass, EUIWidgetContext WidgetContext, APlayerController* PC = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetInputModeUIOnly(APlayerController* PC, UUserWidget* FocusedWidget = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameOnly(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void SetCursorVisible(APlayerController* PC, bool bVisible);

	UFUNCTION(BlueprintCallable)
	void SetPause(bool bPause);

	UFUNCTION(BlueprintCallable)
	void ToggleESCMenu(APlayerController* PC); // bonus
	
};
