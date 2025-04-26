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

	OwnerCharacter = Cast<AGAS_CharacterBase>(GetOwner());
	if (!OwnerCharacter) 
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is null in: %s"), *GetName());
		return;
	}

	BindCharacterDeSpawn();
}

void UAC_RespawnBase::BindCharacterDeSpawn()
{
	// The logic will be implemented in the subclasses.
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

void UAC_RespawnBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CharacterDeSpawnCountDownTimerHandle.IsValid() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CharacterDeSpawnCountDownTimerHandle);
	}
}






