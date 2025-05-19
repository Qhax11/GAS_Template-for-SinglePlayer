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


};
