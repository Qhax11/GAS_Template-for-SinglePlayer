// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_HeroBase.generated.h"

class AGAS_HeroBase;
class UGAS_AbilitySystemComponent;
class UAC_TagDelegates;

UCLASS(Abstract)
class GAS_TEMPLATESP_API UAC_HeroBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	UGAS_AbilitySystemComponent* HeroASC;

	UPROPERTY()
	UAC_TagDelegates* HeroTagDelegatesComp;
};
