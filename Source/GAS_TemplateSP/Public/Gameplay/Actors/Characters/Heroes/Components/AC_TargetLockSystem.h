// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_TargetLockSystem.generated.h"


// TODO: Target'ýn saðýndaki ve solundaki birimleri hesaplayarak en yakýnda birimi yeni hedef olarak alýcak algoritma lazým
// dot vektörü ile yapabilirsin, gpt'ye sor..

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

	void StartTargetLock();

	void EndTargetLock();

	void TryToFindNewTarget(TEnumAsByte<ETargetChangeDirection> TargetChangeDirection);

	void SplitActorsByPositionRelativeToHero(const TArray<AActor*>& InActors, TArray<AActor*>& OutLeftActors, TArray<AActor*>& OutRightActors);

	AActor* FindNearestActor(AActor* TargetedActor, TArray<AActor*> ActorArray);

	void ChangeTarget(AActor* NewTarget);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void RotateCameraToTarget();

	void RotateHeroToTarget();

public:

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|TraceDate")
	class UGAS_AbilityTraceData* TracingDataStart;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|TraceDate")
	class UGAS_AbilityTraceData* TracingDataTargetChange;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|TraceDate")
	class UGAS_AbilityTraceData* TracingDataCheckForFrontActor;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|TraceDate")
	bool bEnableTraceDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* ActivateTargetLockInput;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Inputs")
	const UInputAction* LookMouseInput;

	// Represents the movement speed threshold for horizontal mouse movement.
    // Used to determine the sensitivity for shifting the target lock left or right.
	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem", meta = (ToolTip = "Defines the sensitivity threshold for horizontal mouse movement when shifting the target lock left or right."))
	float Threshold = 1.0f;

	// Cooldown Mechanism: Each direction can trigger the action only once per ExecutionCooldownHorizontal duration.
	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem", meta = (ToolTip = "Time interval within which each direction can trigger the action only once."))
	float ExecutionCooldownHorizontal = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Rotate")
	float RotateInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLockSystem|Rotate")
	float CameraOffsetZ = 100.0f;

protected:

	UPROPERTY(BlueprintReadWrite)
	bool bLocked = false;

	AGAS_HeroBase* HeroBase;
	UAbilitySystemComponent* HeroASC;

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentTarget;
	UAbilitySystemComponent* CurrentTargetASC;

private:

	float LastExecutionTimeRight = 0.0f;
	float LastExecutionTimeLeft = 0.0f;
};
