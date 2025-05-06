// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
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
        UAbilitySystemComponent* InEnemyASC,
        AGAS_HeroBase* InHero,
        UAC_HeroMovementListener* InHeroMovementListener,
        EBehaviorState InBehaviorState)
        : Asset(InAsset)
        , Enemy(InEnemy)
        , EnemyASC(InEnemyASC)
        , Hero(InHero)
        , HeroMovementListenerComp(InHeroMovementListener)
        , BehaviorState(InBehaviorState)
    {}

    FBehaviorServiceInitParams() = default;
};

UCLASS()
class GAS_TEMPLATESP_API UBehaviorDecisionServiceBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(const FBehaviorServiceInitParams& BehaviorServiceInitParams);

protected:
    float GetTargetDistance();

    AGAS_EnemyBase* Enemy;
    UAbilitySystemComponent* EnemyASC;
    AGAS_HeroBase* Hero;
	UAC_HeroMovementListener* HeroMovementListenerComp;
	EBehaviorState BehaviorState;
};
