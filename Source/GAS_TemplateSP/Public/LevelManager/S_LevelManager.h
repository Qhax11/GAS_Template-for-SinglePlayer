// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_LevelManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API US_LevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnPlayerControllerSpawn(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void OpenLevelByName(FName LevelName);

	UFUNCTION(BlueprintCallable)
	bool IsCurrentLevel(FName LevelName) const;
	
protected:
	FString GetCleanLevelName() const;

	class US_UIManager* UIManager;
	const class UDS_LevelManager* LevelManagerSettings;

	UPROPERTY()
	FName CurrentLevelName;
};
