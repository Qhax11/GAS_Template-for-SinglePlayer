// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTypes.generated.h"

UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
    None,
    Passive,
    Aggressive,
    Defensive,
    Confused
};