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
	void OnPlayerControllerSpawn(const APlayerController* PC);

	void CreateLevelWidget(const APlayerController* PC, const TSubclassOf<UUserWidget>* Widget);

	UFUNCTION(BlueprintCallable)
	void OpenLevelByName(FName LevelName);
	
protected:
	const class UDS_LevelManager* LevelManagerSettings;

	UPROPERTY()
	FName CurrentLevelName;
};
