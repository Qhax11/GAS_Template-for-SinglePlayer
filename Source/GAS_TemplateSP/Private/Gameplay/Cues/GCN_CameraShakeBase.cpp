// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/GCN_CameraShakeBase.h"
#include "Gameplay/Components/AC_GameplayData.h"
#include "Gameplay/Abilities/Attack/GA_MeleeAttackBase.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include <Kismet/GameplayStatics.h>

AGCN_CameraShakeBase::AGCN_CameraShakeBase()
{
	GameplayCueTag = GAS_Tags::TAG_GameplayCue_Combat_CameraShake;
}

void AGCN_CameraShakeBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Source)
	{
		return;
	}

	const UGA_MeleeAttackBase* MeleeAttackBase = Cast<UGA_MeleeAttackBase>(Parameters.EffectContext.GetAbility());
	if (!MeleeAttackBase)
	{
		return;
	}

	FGameplayTag AttackType = MeleeAttackBase->GetAttackTypeTagFromAbilityTags();

	UAC_GameplayData* DataComponent = Source->GetComponentByClass<UAC_GameplayData>();
	if (!DataComponent)
	{
		return;
	}

	float CameraShakeForce = 1.0f;
	UDA_AttackTypeToCameraShake* AttackTypeToCameraShakeData = DataComponent->GetAttackTypeToCameraShakeData();
	if (AttackTypeToCameraShakeData)
	{
		CameraShakeForce = AttackTypeToCameraShakeData->FindCameraShakeForce(AttackType);
	}

	// Get player controller of the local player
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(Source->GetWorld(), 0))
	{
		PC->ClientStartCameraShake(CameraShakeClass, CameraShakeForce);
	}
}
