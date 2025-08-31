// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/Camera/GCN_CameraShakeBase.h"


AGCN_CameraShakeBase::AGCN_CameraShakeBase()
{
	GameplayCueTag = GAS_Tags::TAG_GameplayCue_Combat_Camera_Shake;
}

bool AGCN_CameraShakeBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Super::OnExecuted(Source, Target, Parameters))
	{
		return false;
	}

	float CameraShakeForce = 1.0f;
	UDA_AttackTypeToCameraShake* AttackTypeToCameraShakeData = SourceDataComponent->GetAttackTypeToCameraShakeData();
	if (AttackTypeToCameraShakeData)
	{
		CameraShakeForce = AttackTypeToCameraShakeData->FindCameraShakeForce(SourceMeleeAttackType);
	}

	HeroGameplayCameraComponent->ShakeCamera(CameraShakeForce);
	return true;
}
