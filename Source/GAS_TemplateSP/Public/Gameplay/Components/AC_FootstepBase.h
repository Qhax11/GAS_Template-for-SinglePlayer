// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_FootstepBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_FootstepBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_FootstepBase();

protected:
	virtual void BeginPlay() override;

	class AGAS_CharacterBase* OwnerCharacter;
	class UAbilitySystemComponent* OwnerASC;
		
	/** List of tags to match against gameplay events */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer EventTags;

	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const;
};
