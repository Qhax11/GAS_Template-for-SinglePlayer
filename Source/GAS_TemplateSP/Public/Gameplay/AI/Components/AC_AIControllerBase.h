// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_AIControllerBase.generated.h"

/**
 * Base class for all AI Controller components.
 *
 * This component serves as the foundation for AI-related components that are attached to AIControllerBase.
 * It provides common initialization logic and cached references to frequently accessed objects
 * (such as OwnerController, OwnerEnemyBase, OwnerEnemyASC) to avoid repeated casting and null checks.
 *
 * Child classes (UAC_StateManager, UAC_BehaviorDecision, etc.) inherit these cached references
 * and can override OnAICacheReady() to perform additional initialization once the cache is validated.
 *
 * Usage:
 * - Create a new component class that inherits from UAC_AIControllerBase
 * - Override OnAICacheReady() to add component-specific initialization
 * - Access cached references (OwnerController, OwnerEnemyBase, etc.) directly without additional casting
 */

class AAIControllerBase;
class AGAS_EnemyBase;
class UGAS_AbilitySystemComponent;
class AGAS_HeroBase;
class UAC_EnemyMovementManager;
class UAC_IntendManager;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_AIControllerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_AIControllerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AAIControllerBase* OwnerController;

	UPROPERTY()
	AGAS_EnemyBase* OwnerEnemyBase;

	UPROPERTY()
	UGAS_AbilitySystemComponent* OwnerEnemyASC;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	UAC_EnemyMovementManager* MovementManager;

	UPROPERTY()
	UAC_IntendManager* IntendManager;
};
