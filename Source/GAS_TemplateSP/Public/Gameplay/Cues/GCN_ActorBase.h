// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameplayCueNotify_Actor.h"
#include "GCN_ActorBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGCN_ActorBase : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
	
public:
	// Default function implementations
	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) final;

	bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) final;

	bool WhileActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) final;

	bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) final;

protected:
	// Custom functions with flexibility for further modification
	virtual void OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	virtual void OnActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	virtual void OnWhileActivated(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);

	virtual void OnRemoved(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters);
};
