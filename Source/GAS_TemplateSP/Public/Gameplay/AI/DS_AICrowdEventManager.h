// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Engine/DeveloperSettings.h"
#include "DS_AICrowdEventManager.generated.h"


UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "AICrowdEventManager"))
class GAS_TEMPLATESP_API UDS_AICrowdEventManager : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "CrowdAttackingParams")
	int32 MaxEnemyAttackingCount = 2;
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "CrowdAttackingParams")
	bool bDebug = false;
};
