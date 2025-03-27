// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "AC_MovementListener.generated.h"

// Recommendation: make function that calculate how far have you traveled for ai.

UENUM(BlueprintType)
enum class ERelativeDirection : uint8
{
	None     UMETA(DisplayName = "None"),
	Forward  UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Left     UMETA(DisplayName = "Left"),
	Right    UMETA(DisplayName = "Right")
};

USTRUCT()
struct FMotionSample
{
	GENERATED_BODY()

	FVector Location;
	float Time;

	FMotionSample() : Location(FVector::ZeroVector), Time(0.f) {}
	FMotionSample(FVector InLocation, float InTime) : Location(InLocation), Time(InTime) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_MovementListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_MovementListener();

protected:
	virtual void BeginPlay() override;

	ACharacter* OwnerCharacter;
	class UCharacterMovementComponent* OwnerMovementComp;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool HasMovedInLastSeconds(float Seconds) const;

	UFUNCTION(BlueprintCallable)
	float GetDisplacementInLastSeconds(float Seconds) const;

	UFUNCTION(BlueprintCallable)
	ERelativeDirection GetRelativeMovementDirection(float Seconds, AActor* ReferenceActor) const;
	FVector GetDisplacementDirectionInLastSeconds(float Seconds) const;

private:
	float LastMovementTime = 0.0f;
	TArray<FMotionSample> MotionHistory;
	float MaxHistoryTime = 2.0f;

	float SampleInterval = 0.1f;
	float TimeSinceLastSample = 0.0f;
};
