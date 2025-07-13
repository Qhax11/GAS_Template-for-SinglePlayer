// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"
#include "AC_RespawnBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterReSpawn, AGAS_CharacterBase*, Character);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_RespawnBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_RespawnBase();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterReSpawn OnCharacterReSpawn;

protected:
	virtual void BeginPlay() override;

	virtual void BindCharacterDeSpawn();

	void ApplyCharacterReSpawnEffect(AGAS_CharacterBase* CharacterBase);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle CharacterDeSpawnCountDownTimerHandle;

	UPROPERTY(EditAnywhere, Category = "RespawnBase")
	float ReSpawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "RespawnBase")
	bool bDisableSpawn = false;

	UPROPERTY(EditDefaultsOnly, Category = "RespawnBase")
	TSubclassOf<class UGameplayEffect> ReSpawnEffectClass;

	AGAS_CharacterBase* OwnerCharacter;
};
