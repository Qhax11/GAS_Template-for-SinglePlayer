// Qhax's GAS Template for SinglePlayer

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NativeGameplayTags.h"
#include "ComboPreActivationData.generated.h"

/**
 * UObject-based payload used to pass combo-specific pre-activation context to abilities via GameplayEvents.
 *
 * Purpose:
 * - Carries external, time-sensitive data that must be available to the ability BEFORE execution logic
 *   (e.g. before montage notifies or motion warping is triggered).
 * - Avoids race conditions caused by writing directly to ability instances or specs.
 *
 * Typical Usage:
 * - Created by a Combo / Shadow / Attack manager.
 * - Sent through ASC as GameplayEvent (EventData.OptionalObject).
 * - Consumed by combo-related abilities (e.g. ComboMeleeAttack) to configure montage section
 *   and motion warping targets deterministically.
 *
 * Notes:
 * - This object is NOT responsible for activating abilities.
 * - It only provides contextual data; abilities may still be activated normally.
 * - UObject is required (USTRUCT cannot be used) because GameplayEventData only supports UObject payloads.
 */
UCLASS(BlueprintType)
class GAS_TEMPLATESP_API UComboPreActivationData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Initializes all pre-activation data in a single call (BP-friendly). */
	UFUNCTION(BlueprintCallable)
	UComboPreActivationData* Initialize(
		FName InComboMontageSection,
		FGameplayTag InAdditionalTag,
		FVector InMotionWarpingLocation,
		FRotator InMotionWarpingRotation
	);

	UPROPERTY(BlueprintReadWrite)
	FName ComboMontageSection = NAME_None;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag AdditionalTag;

	UPROPERTY(BlueprintReadWrite)
	FVector MotionWarpingLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FRotator MotionWarpingRotation = FRotator::ZeroRotator;
};
