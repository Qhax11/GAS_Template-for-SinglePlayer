// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, FName, LevelName);

UCLASS()
class GAS_TEMPLATESP_API US_LevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void HandlePostLoadMap(UWorld* LoadedWorld);

	UFUNCTION(BlueprintCallable)
	void OpenLevel(FName LevelName, bool bShowLoadingScreen);

	UFUNCTION(BlueprintCallable)
	bool IsCurrentLevel(FName LevelName) const;
	
	UFUNCTION(BlueprintCallable)
	FName GetCleanLevelName() const;

	UPROPERTY(BlueprintAssignable)
	FOnLevelChanged OnLevelChanged;

	UPROPERTY(BlueprintReadOnly)
	bool bShowStartupLoading = false;

protected:
	const class UDS_LevelManager* LevelManagerSettings;

	UPROPERTY()
	FName CurrentLevelName;

	UPROPERTY()
	FName LastLevelName;
};
