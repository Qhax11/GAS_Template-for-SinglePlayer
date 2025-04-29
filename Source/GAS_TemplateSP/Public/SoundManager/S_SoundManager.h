// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "S_SoundManager.generated.h"


UCLASS()
class GAS_TEMPLATESP_API US_SoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void FirstInit();

	UFUNCTION()
	void OnLevelChanged(FName LevelName);

	void PlayMusicToLevel(FName LevelName);

private:
	const class UDS_SoundManager* SoundManagerSettings;
	class US_LevelManager* LevelManager;
};
