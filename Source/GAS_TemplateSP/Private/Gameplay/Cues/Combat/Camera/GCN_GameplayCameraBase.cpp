// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Cues/Combat/Camera/GCN_GameplayCameraBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"

bool AGCN_GameplayCameraBase::OnExecuted(AActor* Source, AActor* Target, const FGameplayCueParameters& Parameters)
{
	if (!Super::OnExecuted(Source, Target, Parameters))
	{
		return false;
	}

	// Check Source
	if(SourceASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
	{
		ExecuteCameraEffect(Source);
		return true;
	}

	// Check Target
	if (TargetASC->HasMatchingGameplayTag(GAS_Tags::TAG_Gameplay_Entity_Character_Hero))
	{
		ExecuteCameraEffect(Target);
		return true;
	}

	return false;
}

void AGCN_GameplayCameraBase::ExecuteCameraEffect(AActor* HeroActor)
{
	AGAS_HeroBase* Hero = Cast<AGAS_HeroBase>(HeroActor);
	if (!Hero) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero is null in: %s"), *GetName());
		return;
	}

	HeroGameplayCameraComponent = Hero->GetHeroGameplayCameraComponent();
	if (!HeroGameplayCameraComponent) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroGameplayCameraComponent is null in: %s"), *GetName());
		return;
	}

	if (GameplayCueTag == GAS_Tags::TAG_GameplayCue_Combat_Camera_Shake)
	{
		float CameraShakeForce = 1.0f;
		UDA_AttackTypeToCameraShake* AttackTypeToCameraShakeData = SourceDataComponent->GetAttackTypeToCameraShakeData();
		if (AttackTypeToCameraShakeData)
		{
			CameraShakeForce = AttackTypeToCameraShakeData->FindCameraShakeForce(SourceMeleeAttackType);
		}
		HeroGameplayCameraComponent->ShakeCamera(CameraShakeForce);
	}
	else if (GameplayCueTag == GAS_Tags::TAG_GameplayCue_Combat_Camera_ZoomIn)
	{
		UDA_AttackTypeToCameraZoom* AttackTypeToCameraZoomData = SourceDataComponent->GetAttackTypeToCameraZoomData();
		HeroGameplayCameraComponent->StartCameraZoomIn();
	}
	else if (GameplayCueTag == GAS_Tags::TAG_GameplayCue_Combat_Camera_ZoomOut)
	{
		float ZoomAmount = 100.0f;
		float ZoomDuracion = 0.2f;
		UDA_AttackTypeToCameraZoom* AttackTypeToCameraZoomData = SourceDataComponent->GetAttackTypeToCameraZoomData();
		if (AttackTypeToCameraZoomData)
		{
			ZoomAmount = AttackTypeToCameraZoomData->FindZoomAmount(SourceMeleeAttackType);
			ZoomDuracion = AttackTypeToCameraZoomData->FindZoomDuration(SourceMeleeAttackType);
			HeroGameplayCameraComponent->StartCameraZoomOut();
		}
	}
}




