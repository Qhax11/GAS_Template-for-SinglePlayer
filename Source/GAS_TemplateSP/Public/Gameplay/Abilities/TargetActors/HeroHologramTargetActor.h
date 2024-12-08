// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/TargetActors/HologramTargetActorBase.h"
#include "HeroHologramTargetActor.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AHeroHologramTargetActor : public AHologramTargetActorBase
{
	GENERATED_BODY()

public:

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AGAS_HeroBase* HeroBase;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USC_EyeOfView* EyeOfViewComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAC_TargetLockSystem* TargetLockSystemComponent;
};
