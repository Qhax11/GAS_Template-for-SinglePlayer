// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroInteraction();


protected:
	virtual void BeginPlay() override;

	void TryBindInteractionInputs();

	UPROPERTY(EditAnywhere, Category = "UI")
	const UInputAction* IA_ToggleMenu;
		
	AGAS_HeroBase* HeroBase;

	UFUNCTION()
	void OnToggleMenuPressed();

private:
	class US_UIManager* UIManager;

};
