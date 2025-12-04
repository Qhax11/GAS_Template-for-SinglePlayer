// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroMovementListener.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "Gameplay/AI/DataTypes/Behavior/BehaviorTypes.h"
#include "Gameplay/AI/DataTypes/Behavior/MovementChainData.h"
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
	EBehaviorState BehaviorState = EBehaviorState::None;

    FBehaviorServiceInitParams(
        AGAS_EnemyBase* InEnemy,
        AAIControllerBase* InEnemyController,
        UAbilitySystemComponent* InEnemyASC,
        AGAS_HeroBase* InHero,
        UAC_HeroMovementListener* InHeroMovementListener,
        EBehaviorState InBehaviorState)
        : Enemy(InEnemy)
        , EnemyController(InEnemyController)
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
