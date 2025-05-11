// Qhax's GAS Template for SinglePlayer

#pragma once

#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeTypes.h"
#include "Gameplay/AI/States/StateBase.h"
#include "Gameplay/AI/Components/AC_StateManager.h"
#include "STT_Test.generated.h"

// For just testing how works state tree tasks in C++

USTRUCT(BlueprintType)
struct FStartStateInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AGAS_EnemyBase> EnemyBase = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AAIControllerBase> EnemyController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UStateBase> StateClass;
};

USTRUCT(meta = (DisplayName = "Test"))
struct FPrintActorNameTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStartStateInstanceData;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual const UStruct* GetInstanceDataType() const override { return FStartStateInstanceData::StaticStruct(); }
};