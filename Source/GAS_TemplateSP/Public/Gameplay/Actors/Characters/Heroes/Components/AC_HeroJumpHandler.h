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
	float JumpHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	float GroundJumpForwardStrength = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	float AirControlStrength = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	int32 MaxJumpCount = 2; 

	UPROPERTY(EditDefaultsOnly, Category = "Jump|Settings")
	bool bDebugPhases = false;

	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	TSubclassOf<UGameplayEffect> GhostEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	FGameplayTagContainer JumpMontageBlockedTags;

	EJumpPhase GetCurrentPhase() const { return CurrentPhase; }

	bool IsInAir() const;

private:
	void SetPhaseAndPlayMontageWithDelay(EJumpPhase NewPhase, float Delay);

	void SetPhaseAndPlayMontage(EJumpPhase NewPhase);

	bool SetPhase(EJumpPhase NewPhase);

	float PlayMontage(UAnimMontage* MontageToPlay);

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	UPROPERTY()
	UAnimInstance* HeroAnimInstance;

	UPROPERTY()
	AGAS_HeroBase* HeroBase;

	UPROPERTY()
	UAbilitySystemComponent* HeroASC;

	UPROPERTY()
	class UAC_HeroControl* HeroControl;

	UPROPERTY()
	UCharacterMovementComponent* HeroMovement;

	EJumpPhase CurrentPhase = EJumpPhase::None;
	int32 JumpCount = 0;
	FTimerHandle PhaseTransitionTimerHandle;
};
