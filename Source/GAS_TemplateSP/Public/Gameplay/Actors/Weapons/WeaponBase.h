// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "WeaponBase.generated.h"

class UAC_TagDelegates;

UCLASS()
class GAS_TEMPLATESP_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	// Trace start and end getters
	UFUNCTION(BlueprintCallable)
	FVector GetTraceStart() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTraceMid() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTraceEnd() const;

	FRotator GetTraceEndRotation() const;

	// Frame-to-frame swing direction
	UFUNCTION(BlueprintCallable, Category = "Weapon|Tracking")
	FVector GetSwingDirection() const { return SwingDirection; }

protected:
	UFUNCTION()
	void OnPhaseActivePostHitTagAdded(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	UFUNCTION()
	void OnPhaseActivePostHitTagRemoved(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& Tag);

	void EnableTracking();
	void DisableTracking();

protected:
	// Weapon mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	// Start point for melee trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Trace")
	USceneComponent* TraceStart;

	// Mid point for melee trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Trace")
	USceneComponent* TraceMid;

	// End point for melee trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Trace")
	USceneComponent* TraceEnd;

	// Tracking state
	UPROPERTY()
	bool bIsTracking = false;

	// Previous frame trace mid position
	FVector PreviousMid = FVector::ZeroVector;

	// Current computed swing direction
	UPROPERTY(BlueprintReadOnly)
	FVector SwingDirection = FVector::ZeroVector;
};
