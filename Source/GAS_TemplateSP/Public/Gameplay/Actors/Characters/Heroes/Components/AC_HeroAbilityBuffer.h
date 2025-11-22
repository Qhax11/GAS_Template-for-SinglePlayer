// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroBase.h"
#include "GameplayTagContainer.h"
#include "AC_HeroAbilityBuffer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroAbilityBuffer : public UAC_HeroBase
{
	GENERATED_BODY()

public:	
	UAC_HeroAbilityBuffer();

protected:
	virtual void BeginPlay() override;
		
	UFUNCTION()
	void OnAbilityFailed(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& TagExplaining);

};
