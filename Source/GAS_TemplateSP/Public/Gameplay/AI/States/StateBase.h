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
    UAC_TagDelegates* EnemyTagDelegatesComp = nullptr;

    UPROPERTY()
    UAC_BehaviorDecision* BehaviorDecisionComponent = nullptr;

    UPROPERTY()
    AActor* HeroTarget = nullptr;

    UPROPERTY()
    UGAS_AbilitySystemComponent* HeroTargetASC = nullptr;

    UPROPERTY()
    UAC_StateManager* StateManager = nullptr;

    FStateInitParams(
        AGAS_EnemyBase* InEnemy,
        AAIControllerBase* InEnemyController, 
        UGAS_AbilitySystemComponent* InEnemyASC,
        UAC_TagDelegates* InEnemyTagDelegatesComp,
        UAC_BehaviorDecision* InBehaviorDecisionComponent,
        AActor* InHeroTarget,
        UGAS_AbilitySystemComponent* InHeroTargetASC,
        UAC_StateManager* InStateManager)
        :
        Enemy(InEnemy),
        EnemyController(InEnemyController),
        EnemyASC(InEnemyASC),
        EnemyTagDelegatesComp(InEnemyTagDelegatesComp),
        BehaviorDecisionComponent(InBehaviorDecisionComponent),
        HeroTarget(InHeroTarget),
        HeroTargetASC(InHeroTargetASC),
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
    bool ExitCondition();
    virtual bool ExitCondition_Implementation() { return true; }

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
    virtual void ExitRequest(FString Reason, const FGameplayTag& TransactionTag = FGameplayTag());

    bool IsAttackInRange(TSubclassOf<class UGAS_GameplayAbilityBase> AbilityClass);

    FAttackData GetSelectedAttackAbilityData() const;

    //FComingAttackReactionData GetSelectedReactionData() const;

    UGAS_GameplayAbilityBase* GetSelectedAttackAbilityCDO() const;

    FAttackData SelectNewAttackAbility() const;

    /**
     * @brief Checks if the call is on the Game Thread. If not, it defers ExitRequest
     * to the Game Thread to prevent data races and crashes.
     * @param ExitReason The string reason passed to ExitRequest.
     * @return True if ExitRequest was called immediately on the Game Thread,
     * False if the request was deferred to the Game Thread.
     */
    bool CheckThreadAndExitSafe(const FString& ExitReason);

protected:
    UPROPERTY(BlueprintReadOnly)
    AGAS_EnemyBase* Enemy;

    UPROPERTY(BlueprintReadOnly)
    AAIControllerBase* EnemyController;

    UPROPERTY(BlueprintReadOnly)
    UGAS_AbilitySystemComponent* EnemyASC;

    UPROPERTY(BlueprintReadOnly)
    UAC_TagDelegates* EnemyTagDelegatesComp;

    UPROPERTY(BlueprintReadOnly)
    UAC_BehaviorDecision* BehaviorDecisionComponent;

    UPROPERTY(BlueprintReadOnly)
    AActor* HeroTarget;

    UPROPERTY(BlueprintReadOnly)
    UGAS_AbilitySystemComponent* HeroTargetASC;

    class UAC_StateManager* StateManager;
};
