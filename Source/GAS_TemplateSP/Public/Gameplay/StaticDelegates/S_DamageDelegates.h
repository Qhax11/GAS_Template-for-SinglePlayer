// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/Effects/Calculations/EC_DamageBase.h"
#include "S_DamageDelegates.generated.h"

class UAbilitySystemComponent;
class AGAS_CharacterBase;

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

public:
	FExecCalculationParameters ExecCalculationParameters;

	bool bParrySucces = false;

	FDamageData() {}

	FDamageData(FExecCalculationParameters InExecCalculationParameters, bool InbParrySucces)
		: ExecCalculationParameters(InExecCalculationParameters), bParrySucces(InbParrySucces)
	{}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, const FDamageData&, DamageData);


UCLASS()
class GAS_TEMPLATESP_API US_DamageDelegates : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// This delegate is triggered by the UEC_DamageBase
	UPROPERTY(BlueprintAssignable)
	FOnDamageDealt OnDamageDealt;

};
