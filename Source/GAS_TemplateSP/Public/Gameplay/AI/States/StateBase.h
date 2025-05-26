// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "StateBase.generated.h"

USTRUCT()
struct FStateInitParams
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FGameplayTag StateTag;

    UPROPERTY()
    AGAS_EnemyBase* Enemy = nullptr;

    UPROPERTY()
    AAIControllerBase* EnemyController = nullptr;

    UPROPERTY()
    UGAS_AbilitySystemComponent* EnemyASC = nullptr;

    UPROPERTY()
    UAC_BehaviorDecision* BehaviorDecisionComponent = nullptr;

    UPROPERTY()
    UAC_StateManager* StateManager = nullptr;

    FStateInitParams(
        FGameplayTag InStateTag,
        AGAS_EnemyBase* InEnemy, 
        AAIControllerBase* InEnemyController, 
        UGAS_AbilitySystemComponent* InEnemyASC,
        UAC_BehaviorDecision* InBehaviorDecisionComponent,
        UAC_StateManager* InStateManager)
        :
        StateTag(InStateTag),
        Enemy(InEnemy),
        EnemyController(InEnemyController),
        EnemyASC(InEnemyASC),
        BehaviorDecisionComponent(InBehaviorDecisionComponent),
        StateManager(InStateManager)
    {}

    FStateInitParams() = default;
};



UCLASS()
class GAS_TEMPLATESP_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

    virtual void OnEnter();

    virtual void OnTick(float DeltaTime) {}

    virtual void OnExit();

    FGameplayTag StateTag;

protected:
    virtual void ExitRequest(const FGameplayTag& TransactionTag);

    FAttackData GetSelectedAttackAbility() const;

    FAttackData SelectNewAttackAbility() const;

    AGAS_EnemyBase* Enemy;
    AAIControllerBase* EnemyController;
    UGAS_AbilitySystemComponent* EnemyASC;
    UAC_BehaviorDecision* BehaviorDecisionComponent;
    class UAC_StateManager* StateManager;

    bool bStateFinished = false;
};
