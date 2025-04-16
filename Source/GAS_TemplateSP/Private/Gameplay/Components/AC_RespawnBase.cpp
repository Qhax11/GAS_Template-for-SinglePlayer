// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Components/AC_RespawnBase.h"
#include "Gameplay/Effects/GAS_EffectBlueprintFunctionLibary.h"
#include "Gameplay/Actors/Characters/GAS_CharacterBase.h"


UAC_RespawnBase::UAC_RespawnBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_RespawnBase::BeginPlay()
{
	Super::BeginPlay();

	BindCharacterDeSpawn();
}

void UAC_RespawnBase::BindCharacterDeSpawn()
{
	// The logic will be implemented in the subclasses.
}

void UAC_RespawnBase::StartCharacterReSpawnCountdown(const FCharacterDeSpawnData& CharacterDeSpawnData)
{
	if (CharacterDeSpawnData.Character != GetOwner())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(CharacterDeSpawnCountDownTimerHandle, [this, CharacterDeSpawnData]()
		{
			FCharacterSpawnData CharacterSpawnData = FCharacterSpawnData(CharacterDeSpawnData.Character, CharacterDeSpawnData.ASC);
			OnCharacterRespawn(CharacterSpawnData);
		}, 
		ReSpawnDelay, false);
}

void UAC_RespawnBase::OnCharacterRespawn(const FCharacterSpawnData& CharacterSpawnData)
{
	ApplyCharacterReSpawnEffect(CharacterSpawnData.Character);

	CharacterSpawnData.Character->EnableMovement();
	CharacterSpawnData.Character->EnableCollision();
	CharacterSpawnData.Character->EnableMesh();

	OnCharacterReSpawn.Broadcast(CharacterSpawnData.Character);
}

void UAC_RespawnBase::ApplyCharacterReSpawnEffect(AGAS_CharacterBase* Character)
{
	if (!ReSpawnEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReSpawnEffectClass is null in: %s"), *GetName());
		return;
	}

	UGameplayEffect* ReSpawnEffect = UGAS_EffectBlueprintFunctionLibary::CreateEffectWithTSubclass(ReSpawnEffectClass);
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(ReSpawnEffect, 1, FGameplayEffectContextHandle());
}






