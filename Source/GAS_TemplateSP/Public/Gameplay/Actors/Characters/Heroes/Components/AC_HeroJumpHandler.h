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

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Input")
	const UInputAction* IA_ActivateJump;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpStartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpInAirMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages")
	UAnimMontage* JumpLandedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Montages|MultiJump")
	UAnimMontage* DoubleJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float JumpHeight = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 MaxJumpCount = 1; // 1 = tek zýplama, 2 = çift zýplama

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bAutoPlayMontages = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bDebugPhases = false;

	UFUNCTION(BlueprintPure, Category = "Jump")
	EJumpPhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "Jump")
	bool IsInAir() const;

private:
	float PlayMontage(UAnimMontage* MontageToPlay);

	void SetPhase(EJumpPhase NewPhase);

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
