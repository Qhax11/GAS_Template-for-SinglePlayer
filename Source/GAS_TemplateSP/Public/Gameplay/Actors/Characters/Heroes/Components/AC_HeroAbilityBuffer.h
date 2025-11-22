// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroAbilityBuffer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroAbilityBuffer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroAbilityBuffer();

protected:
	virtual void BeginPlay() override;

		
};
