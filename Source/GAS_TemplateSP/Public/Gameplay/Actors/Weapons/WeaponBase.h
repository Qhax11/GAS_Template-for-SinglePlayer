// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class GAS_TEMPLATESP_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Trace start and end getters
	UFUNCTION(BlueprintCallable)
	FVector GetTraceStart() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTraceMid() const;

	UFUNCTION(BlueprintCallable)
	FVector GetTraceEnd() const;

	FRotator GetTraceEndRotation() const;

	// Called from melee attack animation notify to capture the weapon's position each frame for momentum calculations.
	void UpdatePreviousLocation();

	// Stores the weapon's world-space location from the previous frame for calculating swing direction or momentum.
	// Currently used for determining FX spawn direction during hit or slash effects.
	UPROPERTY(BlueprintReadOnly)
	FVector PreviousLocation;

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

};
