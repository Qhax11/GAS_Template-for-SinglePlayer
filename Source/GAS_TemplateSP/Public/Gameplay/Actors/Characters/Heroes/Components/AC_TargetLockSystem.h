// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_TargetLockSystem.generated.h"

UENUM(BlueprintType)
enum ETargetChangeDirection : uint8
{
	Left,
	Right
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class GAS_TEMPLATESP_API UAC_TargetLockSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_TargetLockSystem();

protected:
	virtual void BeginPlay() override;

	void TryBindTargetLockSystemInputs(UEnhancedInputComponent* EnhancedInputComponent);

	void TryActivateTargetLock(const FInputActionValue& Value);

	void LookMouse(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void StartTargetLock();

	UFUNCTION(BlueprintCallable)
	void EndTargetLock();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent)
	void TargetChange(bool right);

	void TryToChangeTarget(TEnumAsByte<ETargetChangeDirection> TargetChangeDirection);

public:

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	class UGAS_AbilityTraceData* TracingDataStart;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	class UGAS_AbilityTraceData* TracingDataLeft;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	class UGAS_AbilityTraceData* TracingDataRight;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* LookMouseInput;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* ActivateTargetLockInput;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem")
	float Threshold;

protected:
	AGAS_HeroBase* HeroBase;
	
	UAbilitySystemComponent* HeroASC;

	UPROPERTY(BlueprintReadWrite)
	bool bLocked = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentTarget;

	UAbilitySystemComponent* CurrentTargetASC;
};
