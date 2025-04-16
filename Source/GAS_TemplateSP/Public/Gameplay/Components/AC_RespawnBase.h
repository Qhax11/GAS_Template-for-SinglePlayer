// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/StaticDelegates/S_SpawnDelegates.h"
#include "AC_RespawnBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterReSpawn, AGAS_CharacterBase*, Character);

class AGAS_CharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_RespawnBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_RespawnBase();

	UFUNCTION()
	void StartCharacterReSpawnCountdown(const FCharacterDeSpawnData& CharacterDeSpawnData);

	UPROPERTY(BlueprintAssignable)
	FOnCharacterReSpawn OnCharacterReSpawn;

protected:
	virtual void BeginPlay() override;

	virtual void BindCharacterDeSpawn();

	virtual void OnCharacterRespawn(const FCharacterSpawnData& CharacterSpawnData);

	void ApplyCharacterReSpawnEffect(AGAS_CharacterBase* CharacterBase);

	FTimerHandle CharacterDeSpawnCountDownTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "RespawnBase")
	float ReSpawnDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "RespawnBase")
	TSubclassOf<class UGameplayEffect> ReSpawnEffectClass;
};
