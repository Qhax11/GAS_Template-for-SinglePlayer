// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_IntendHandlerBase.generated.h"

/**
 * UAC_IntendHandlerBase
 *
 * Base component class responsible for listening to AI signals and intents.
 * Derived classes like UAC_IntendHandlerBoss and UAC_IntendHandlerCrowd
 * implement specific behaviors for different AI types.
 *
 * It processes external stimuli and triggers state transitions via the State Manager.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_IntendHandlerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_IntendHandlerBase();

protected:
	virtual void BeginPlay() override;

		
};
