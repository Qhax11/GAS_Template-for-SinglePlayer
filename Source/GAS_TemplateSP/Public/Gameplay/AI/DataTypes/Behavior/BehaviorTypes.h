// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTypes.generated.h"

UENUM(BlueprintType)
enum class EEnemyIntent : uint8
{
    LowPressure,
    MidPressure,
    HighPressure
};