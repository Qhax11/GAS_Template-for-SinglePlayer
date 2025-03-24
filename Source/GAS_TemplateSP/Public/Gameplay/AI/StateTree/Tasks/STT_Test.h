// Qhax's GAS Template for SinglePlayer

#pragma once

#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STT_Test.generated.h"

// For just testing how works state tree tasks in C++

USTRUCT()
struct FTestPrintActorNameTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> TargetActor = nullptr;
};

USTRUCT(meta = (DisplayName = "Test"))
struct FPrintActorNameTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FTestPrintActorNameTaskInstanceData;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&) const override;

	virtual const UStruct* GetInstanceDataType() const override { return FTestPrintActorNameTaskInstanceData::StaticStruct(); }
};