// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Abilities/GAS_GameplayAbilityBase.h"
#include "GameFramework/RootMotionSource.h"
#include "GA_HeroDash.generated.h"


UCLASS()
class GAS_TEMPLATESP_API UGA_HeroDash : public UGAS_GameplayAbilityBase
{
	GENERATED_BODY()
	

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	FVector GetDashDirection(const FVector2D& LastMovementInput);

	FVector GetDirectionFromLastMovementInput(const FVector2D& LastMovementInput);

	UFUNCTION()
	void OnTaskTimedOut();
	
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float DistanceMultiplier = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float Duration = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	bool bSetNewMovementMode;
	
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	TEnumAsByte<EMovementMode> NewMovementMode = EMovementMode::MOVE_Walking;
	
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	bool bRestrictSpeedToExpected = false;

	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	class UCurveVector* DashCurve;
	
	// What to do with character's Velocity when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	ERootMotionFinishVelocityMode FinishVelocityMode = ERootMotionFinishVelocityMode::ClampVelocity;
	
	// If FinishVelocityMode mode is "SetVelocity", character velocity is set to this value when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	FVector FinishSetVelocity = FVector(0, 0, 0);

	// If FinishVelocityMode mode is "ClampVelocity", character velocity is clamped to this value when root motion finishes 
	UPROPERTY(EditDefaultsOnly, Category = "RootMotionMoveToForce")
	float FinishClampVelocity = 0.f;
	
};
