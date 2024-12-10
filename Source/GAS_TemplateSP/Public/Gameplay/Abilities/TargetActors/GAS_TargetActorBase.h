// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "GAS_TargetActorBase.generated.h"

/**
 * A custom implementation of an ability target system for handling actor targets.
 *
 * The default GAS (Gameplay Ability System) target actor and task system caused issues in my project.
 * Due to limitations and inconsistencies when using GAS's built-in target actor with tasks,
 * I developed my own actor targeting system to have more control and avoid these conflicts.
 * This custom target actor system allows for smoother interactions with abilities and tasks,
 * ensuring better performance and fewer errors when managing targets in gameplay.
 */

UCLASS()
class GAS_TEMPLATESP_API AGAS_TargetActorBase : public AActor
{
	GENERATED_BODY()
	
public:

	virtual void Confirm();

	virtual void Cancel();

};
