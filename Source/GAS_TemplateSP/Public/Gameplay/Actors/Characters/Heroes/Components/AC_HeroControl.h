// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroControl.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroControl : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAC_HeroControl();

protected:

	virtual void BeginPlay() override;

public:	
	void TryBindMovingInputs(UEnhancedInputComponent* EnhancedInputComponent);

	void Move(const FInputActionValue& Value);

	void LookMouse(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere)
	const UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere)
	const UInputAction* LookMouseInputAction;

protected:

	AGAS_HeroBase* HeroBase;
};
