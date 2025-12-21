// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/AI/Components/AC_IntendManager.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "BehaviorDecisionServiceBase.generated.h"


USTRUCT()
struct FBehaviorServiceInitParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AGAS_EnemyBase* Enemy = nullptr;

    UPROPERTY()
    AAIControllerBase* EnemyController = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* EnemyASC = nullptr;

	UPROPERTY()
    AGAS_HeroBase* Hero = nullptr;

    UPROPERTY()
    UAC_HeroMovementListener* HeroMovementListenerComp = nullptr;

	UPROPERTY()
    UAC_IntendManager* IntendManager = nullptr;

    FBehaviorServiceInitParams(
        AGAS_EnemyBase* InEnemy,
        AAIControllerBase* InEnemyController,
        UAbilitySystemComponent* InEnemyASC,
        AGAS_HeroBase* InHero,
        UAC_HeroMovementListener* InHeroMovementListener,
        UAC_IntendManager* InIntendManager)
        : Enemy(InEnemy)
        , EnemyController(InEnemyController)
        , EnemyASC(InEnemyASC)
        , Hero(InHero)
        , HeroMovementListenerComp(InHeroMovementListener)
        , IntendManager(InIntendManager)
    {}

    FBehaviorServiceInitParams() = default;
};

/**
 * UBehaviorDecisionServiceBase
 *
 * Base class for all AI behavior decision services used by UAC_BehaviorDecision.
 *
 * A BehaviorDecisionService represents a SINGLE, focused decision query such as:
 *  - Selecting the best attack
 *  - Selecting the best movement chain
 *  - Selecting the best reaction to an incoming attack
 *
 * Services are NOT autonomous systems.
 * They do NOT own state, tick, or make decisions by themselves.
 * Instead, they operate as stateless (or minimally stateful) evaluators that are:
 *  - Created
 *  - Initialized
 *  - Queried
 * entirely by UAC_BehaviorDecision.
 *
 * Lifecycle:
 *  1) Created by UAC_BehaviorDecision based on a configuration asset
 *  2) Initialized via FBehaviorServiceInitParams
 *  3) Queried for a decision (e.g. GetBestAttack, GetBestReaction)
 *  4) Optionally finalized via InitializeAfterSelection()
 *
 * Responsibilities:
 *  - Evaluate context-specific options using data-driven logic
 *  - Read enemy, hero, ASC, and behavior state information
 *  - Apply deterministic scoring and filtering rules
 *
 * Non-responsibilities:
 *  - Does NOT execute abilities
 *  - Does NOT manage cooldowns
 *  - Does NOT store long-term runtime state
 *  - Does NOT replace the AIController or State system
 *
 * Design notes:
 *  - Services are intentionally decoupled from each other
 *  - All shared context is injected once via FBehaviorServiceInitParams
 *  - Runtime state (e.g. combat memory) must live on the character, not here
 *
 * This separation allows the AI decision system to be:
 *  - Modular
 *  - Debuggable
 *  - Easily extensible with new decision services
 */

UCLASS()
class GAS_TEMPLATESP_API UBehaviorDecisionServiceBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams);

    virtual void InitializeAfterSelection();

protected:
	UPROPERTY()
    AGAS_EnemyBase* Enemy;

    UPROPERTY()
    AAIControllerBase* EnemyController;

    UPROPERTY()
    UAbilitySystemComponent* EnemyASC;

    UPROPERTY()
    AGAS_HeroBase* Hero;

    UPROPERTY()
	UAC_HeroMovementListener* HeroMovementListenerComp;

    UPROPERTY()
    UAC_IntendManager* IntendManager;

    UPROPERTY(EditDefaultsOnly)
    bool bEnableDebug = true;
};
