// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_TargetLockSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GAS_TEMPLATESP_API UAC_TargetLockSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_TargetLockSystem();

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	class UGAS_AbilityTargetingData* TargetingData;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* LookMouseInput;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* ActivateTargetLockInput;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	float Threshold;

	UFUNCTION(BlueprintCallable)
	void StartTargetLock();

	UFUNCTION(BlueprintCallable)
	void EndTargetLock();

protected:
	virtual void BeginPlay() override;

	void TryBindTargetLockSystemInputs(UEnhancedInputComponent* EnhancedInputComponent);

	void LookMouse(const FInputActionValue& Value);

	void TryActivateTargetLock(const FInputActionValue& Value);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent)
	void TargetChange(bool right);

protected:
	AGAS_HeroBase* HeroBase;
	
	UPROPERTY(BlueprintReadWrite)
	bool bLocked = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentTarget;
};
