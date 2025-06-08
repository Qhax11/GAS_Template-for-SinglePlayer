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
    AGAS_EnemyBase* Enemy = nullptr;

    UPROPERTY()
    AAIControllerBase* EnemyController = nullptr;

    UPROPERTY()
    UGAS_AbilitySystemComponent* EnemyASC = nullptr;

    UPROPERTY()
    AActor* HeroTarget = nullptr;

    UPROPERTY()
    UAC_BehaviorDecision* BehaviorDecisionComponent = nullptr;

    UPROPERTY()
    UAC_StateManager* StateManager = nullptr;

    FStateInitParams(
        AGAS_EnemyBase* InEnemy,
        AAIControllerBase* InEnemyController, 
        UGAS_AbilitySystemComponent* InEnemyASC,
        AActor* InHeroTarget,
        UAC_BehaviorDecision* InBehaviorDecisionComponent,
        UAC_StateManager* InStateManager)
        :
        Enemy(InEnemy),
        EnemyController(InEnemyController),
        EnemyASC(InEnemyASC),
        HeroTarget(InHeroTarget),
        BehaviorDecisionComponent(InBehaviorDecisionComponent),
        StateManager(InStateManager)
    {}

    FStateInitParams() = default;
};


UCLASS(Blueprintable, BlueprintType)
class GAS_TEMPLATESP_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

    UFUNCTION(BlueprintNativeEvent, Category = "State")
    bool EnterCondition();
    virtual bool EnterCondition_Implementation() {return true;}

    UFUNCTION(BlueprintNativeEvent, Category = "State")
    void OnEnter();
    virtual void OnEnter_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "State")
    void OnExit();
    virtual void OnExit_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "State")
    void OnTick(float DeltaTime);
    virtual void OnTick_Implementation(float DeltaTime) {};

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (Categories = "AI.State"))
    FGameplayTag StateTag;

protected:
    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "TransactionTag"))
    virtual void ExitRequest(const FGameplayTag& TransactionTag = FGameplayTag());

    bool IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass);

    FAttackData GetSelectedAttackAbilityData() const;

    FComingAttackReactionData GetSelectedReactionData() const;

    UGAS_GameplayAbilityBase* GetSelectedAttackAbilityCDO() const;

    FAttackData SelectNewAttackAbility() const;

    UPROPERTY(BlueprintReadOnly)
    AGAS_EnemyBase* Enemy;

    UPROPERTY(BlueprintReadOnly)
    AAIControllerBase* EnemyController;

    UPROPERTY(BlueprintReadOnly)
    UGAS_AbilitySystemComponent* EnemyASC;

    UPROPERTY(BlueprintReadOnly)
    AActor* HeroTarget;

    UPROPERTY(BlueprintReadOnly)
    UAC_BehaviorDecision* BehaviorDecisionComponent;

    class UAC_StateManager* StateManager;
};
