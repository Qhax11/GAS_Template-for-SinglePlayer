// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroBase.generated.h"


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
	UAbilitySystemComponent* HeroASC;
};
