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
	FVector GetTraceStart() const;
	FVector GetTraceMid() const;
	FVector GetTraceEnd() const;
	FRotator GetTraceEndRotation() const;

	void UpdatePreviousLocation();

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
