// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "Gameplay/AI/Controllers/AIControllerBase.h"
#include "Gameplay/AI/Components/AC_BehaviorDecision.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
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

    FStateInitParams() = default;
};

// 1. BASE STRUCT (Polymorphic)
struct FStatePayloadBase
{
    virtual ~FStatePayloadBase() {} // Virtual destructor is essential!
};

UCLASS(Blueprintable, BlueprintType)
class GAS_TEMPLATESP_API UStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StateInitalize(const FStateInitParams& StateInitParams);

    virtual bool EnterCondition(TSharedPtr<FStatePayloadBase> EnterPayload = nullptr) { return true; }

    virtual void OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload = nullptr);

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
    virtual bool ExitRequest(FString Reason, const FGameplayTag& TransactionTag = FGameplayTag(), TSharedPtr<FStatePayloadBase> ExitPayload = nullptr);

    bool IsAttackInRange(TSubclassOf<UGAS_GameplayAbilityBase> AbilityClass);

    FAttackData GetSelectedAttackAbilityData() const;

    //FComingAttackReactionData GetSelectedReactionData() const;

    UGAS_GameplayAbilityBase* GetSelectedAttackAbilityCDO() const;

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

    UPROPERTY()
    class UAC_StateManager* StateManager;

};
