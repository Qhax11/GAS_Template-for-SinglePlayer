// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/SaveSystem/SaveGames/SaveGameBase.h"
#include "SG_HeroProgress.generated.h"

struct FAbilityData;

UCLASS()
class GAS_TEMPLATESP_API USG_HeroProgress : public USaveGameBase
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FAbilityData> GrantedAbilities;

	UPROPERTY()
	bool bTutorialFinished;
	
};
