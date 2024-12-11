// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GAS_HeroBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGAS_HeroBase : public AGAS_CharacterBase
{
	GENERATED_BODY()

public:
	AGAS_HeroBase(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* HeroInputMappingContext;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UAC_HeroControl* GetHeroControlComponent() const { return HeroControlComponent; }

	FORCEINLINE class USC_HologramAbilityHelper* GetHologramAbilityHelperComponent() const { return HologramAbilityHelperComponent; }

	FORCEINLINE class UAC_TargetLockSystem* GetTargetLockSystemComponent() const { return TargetLockSystemComponent; }
protected:

	//* Components *//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components|Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components")
	class UAC_HeroControl* HeroControlComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components")
	class UAC_HeroRespawn* HeroRespawnComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components")
	class UAC_TargetLockSystem* TargetLockSystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hero|Components")
	class USC_HologramAbilityHelper* HologramAbilityHelperComponent;

private:
	class UAC_AbilityInputBinding* AbilityInputBindingComponent;

};
