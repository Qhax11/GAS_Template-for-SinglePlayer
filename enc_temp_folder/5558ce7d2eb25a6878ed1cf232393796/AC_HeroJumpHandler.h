// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AC_HeroJumpHandler.generated.h"

UENUM(BlueprintType)
enum class EJumpPhase : uint8
{
	None,
	Start,
	InAir,
	DoubleJump,
	Landed
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_HeroJumpHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_HeroJumpHandler();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ============================================================
	// ANA FONKSÝYONLAR
	// ============================================================

	bool BindJumpInput();

	void ActivateJump();

	void StartJump();

	void JumpLogic();

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Input")
	const UInputAction* IA_ActivateJump;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* DoubleJumpStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpInAirMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpLandedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	float JumpHeight = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	int32 MaxJumpCount = 1; // 1 = tek zýplama, 2 = çift zýplama

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	bool bDebugPhases = false;

	EJumpPhase GetCurrentPhase() const { return CurrentPhase; }

	bool IsInAir() const;

private:
	void SetPhaseAndPlayWithDelay(EJumpPhase NewPhase, float Delay);

	void SetPhaseAndPlayMontage(EJumpPhase NewPhase);

	float PlayMontage(UAnimMontage* MontageToPlay);

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	UPROPERTY()
	UAnimInstance* HeroAnimInstance;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	UCharacterMovementComponent* HeroMovement;

	EJumpPhase CurrentPhase = EJumpPhase::None;
	int32 JumpCount = 0;
};
