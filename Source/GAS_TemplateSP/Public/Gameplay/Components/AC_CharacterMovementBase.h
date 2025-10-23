// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "AC_CharacterMovementBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UAC_CharacterMovementBase : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;

};
