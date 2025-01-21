// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstanceBase.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
class AGAS_CharacterBase;

UCLASS()
class GAS_TEMPLATESP_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstanceBase();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(const FGameplayTag GameplayTag) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	AGAS_CharacterBase* OwnerCharacterBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* OwnerASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* CharacterMovement;

	// Speed of the Chracter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	// Whether or not chacter is in the air
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	//Whether or not chracter is moving
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LocomationDirection;

};
