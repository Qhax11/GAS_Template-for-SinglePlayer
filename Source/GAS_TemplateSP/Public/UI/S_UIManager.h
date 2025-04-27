// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/DS_UIManager.h"
#include "S_UIManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API US_UIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnPlayerControllerSpawn(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateAndShowWidget(const FWidgetData& WidgetData, APlayerController* PC = nullptr);

	void ApplyWidgetContextInputSettings(APlayerController* PC, EUIWidgetContext Context, UUserWidget* FocusedWidget);

	void ApplyPauseBehavior(EGamePauseBehavior PauseBehavior);

	UFUNCTION(BlueprintCallable)
	void SetInputModeUIOnly(APlayerController* PC, UUserWidget* FocusedWidget = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameOnly(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameAndUI(APlayerController* PC, UUserWidget* FocusedWidget = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetCursorVisible(APlayerController* PC, bool bVisible);

	UFUNCTION(BlueprintCallable)
	void SetPause(bool bPause);

	UFUNCTION(BlueprintCallable)
	void ToggleESCMenu(); // bonus
	
	UUserWidget* ESCMenuWidget;
private:
	APlayerController* PlayerController;
	const class UDS_UIManager* UIManagerSettings;

};
