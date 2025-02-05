// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Gameplay/Actors/Characters/Enemies/GAS_EnemyBase.h"
#include "BossBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API ABossBase : public AGAS_EnemyBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

};
