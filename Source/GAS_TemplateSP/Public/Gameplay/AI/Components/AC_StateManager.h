// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_StateManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_StateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_StateManager();

protected:
	virtual void BeginPlay() override;

};
