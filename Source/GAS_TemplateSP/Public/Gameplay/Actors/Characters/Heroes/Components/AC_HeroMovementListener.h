// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AC_HeroMovementListener.generated.h"

// Recommendation: make function that calculate how far have you traveled for ai.

UENUM(BlueprintType)
enum class EHeroRelativeDirection : uint8
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
class GAS_TEMPLATESP_API UAC_HeroMovementListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroMovementListener();

protected:
	virtual void BeginPlay() override;

	class AGAS_HeroBase* OwnerHero;
	class UCharacterMovementComponent* OwnerMovementComp;
	class UAC_HeroControl* HeroControlComp;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool HasMovedInLastSeconds(float Seconds) const;

	UFUNCTION(BlueprintCallable)
	float GetDisplacementInLastSeconds(float Seconds) const;

	UFUNCTION(BlueprintCallable)
	EHeroRelativeDirection GetHeroLastMovementDirectionByLastInput() const;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetHeroLastMovementDirectionTagByLastInput() const;

private:
	float LastMovementTime = 0.0f;
	TArray<FMotionSample> MotionHistory;
	float MaxHistoryTime = 2.0f;

	float SampleInterval = 0.1f;
	float TimeSinceLastSample = 0.0f;
};
