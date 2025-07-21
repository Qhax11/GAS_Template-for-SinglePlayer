// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "BehaviorDecisionServiceBase.generated.h"

UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
    None        UMETA(DisplayName = "None"),
    Passive     UMETA(DisplayName = "Passive"),
    Aggressive  UMETA(DisplayName = "Aggressive"),
    Defensive   UMETA(DisplayName = "Defensive"),
    Confused    UMETA(DisplayName = "Confused")
};

USTRUCT()
struct FBehaviorServiceInitParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UObject* Asset = nullptr;

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
	EBehaviorState BehaviorState = EBehaviorState::None;

    FBehaviorServiceInitParams(
        UObject* InAsset,
        AGAS_EnemyBase* InEnemy,
        AAIControllerBase* InEnemyController,
        UAbilitySystemComponent* InEnemyASC,
        AGAS_HeroBase* InHero,
        UAC_HeroMovementListener* InHeroMovementListener,
        EBehaviorState InBehaviorState)
        : Asset(InAsset)
        , Enemy(InEnemy)
        , EnemyController(InEnemyController)
        , EnemyASC(InEnemyASC)
        , Hero(InHero)
        , HeroMovementListenerComp(InHeroMovementListener)
        , BehaviorState(InBehaviorState)
    {}

    FBehaviorServiceInitParams() = default;
};

USTRUCT(BlueprintType)
struct FMovementAbilityData
{
    GENERATED_BODY()

public:
    // The gameplay ability class used for movement.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGAS_GameplayAbilityBase> MovementAbilityClass;

    // The gameplay tag used to trigger this ability.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.AbilityTriggerEvent.Movement"))
    FGameplayTag AbilityTriggerTag;

    // The resolved direction for this ability, typically determined at runtime by a direction policy.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Resolved", ToolTip = "Resolved direction tag used by this ability at runtime. Typically set based on the direction policy."))
    FGameplayTag ResolvedDirectionTag;

    // The policy used to resolve the direction, like random or based on player position.
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "AI.Direction.Policy", ToolTip = "Defines how the direction should be resolved at runtime (e.g., LastPlayerDirection, Random)."))
    FGameplayTag DirectionPolicyTag;

    // Used as movement distance for dash abilities, or as a time limit (in seconds) for other movement types like chase or flee.
    // Set to 0 to ignore. For dash, this represents how far the actor should move. For chase/flee, this limits how long the ability stays active.
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Used as movement distance for dash abilities, or as a time limit (in seconds) for chase/flee behaviors. Set to 0 to ignore."))
    float AbilityEventMagnitude = 0.f;
};

UCLASS()
class GAS_TEMPLATESP_API UBehaviorDecisionServiceBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams);

    virtual void InitializeAfterSelection();

protected:
    AGAS_EnemyBase* Enemy;
    AAIControllerBase* EnemyController;
    UAbilitySystemComponent* EnemyASC;
    AGAS_HeroBase* Hero;
	UAC_HeroMovementListener* HeroMovementListenerComp;
	EBehaviorState BehaviorState;

protected:
    // Movement
    FGameplayTag GetRandomDirectionTag();

    void ApplyDirectionPoliciesToMovementAbility(FMovementAbilityData& MovementAbilityData);

};
