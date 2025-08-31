// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/Camera/GCN_CameraShakeBase.h"
#include <Kismet/GameplayStatics.h>

AGCN_CameraShakeBase::AGCN_CameraShakeBase()
{
	GameplayCueTag = GAS_Tags::TAG_GameplayCue_Combat_Camera_Shake;
}

void AGCN_CameraShakeBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	Super::OnExecuted(Source, Target, Parameters);

	if (!Source)
	{
		return;
	}

	float CameraShakeForce = 1.0f;
	UDA_AttackTypeToCameraShake* AttackTypeToCameraShakeData = SourceDataComponent->GetAttackTypeToCameraShakeData();
	if (AttackTypeToCameraShakeData)
	{
		CameraShakeForce = AttackTypeToCameraShakeData->FindCameraShakeForce(SourceMeleeAttackType);
	}

	// Get player controller of the local player
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(Source->GetWorld(), 0))
	{
		PC->ClientStartCameraShake(CameraShakeClass, CameraShakeForce);
	}
}
