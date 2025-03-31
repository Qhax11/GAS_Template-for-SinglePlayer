// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMeleeComboManager.h"
#include "Gameplay/Abilities/TargetActors/Shadows/BossShadowTargetActor.h"
#include "Gameplay/AI/StateTree/ST_Base.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_BossShadowAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FTimerHandle ConfirmTimerHandle;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ConfirmTimerHandle,                      
			this,                              
			&UGA_BossShadowAttack::OnTimerConfirm,  
			ExecuteTime,                              
			false                              
		);
	}
}

void UGA_BossShadowAttack::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
	FVector ShadowSpawnLocation = Location;

	if (AActor* Target = BossController->GetTarget()) 
	{
		FVector TargetLocation = Target->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ShadowSpawnLocation, TargetLocation);
		Super::SpawnAndSetupTargetActor(FRotator(0, LookAtRotation.Yaw, 0), ShadowSpawnLocation);
	}
	else
	{
		Super::SpawnAndSetupTargetActor(Rotation, ShadowSpawnLocation);
	}
}

void UGA_BossShadowAttack::OnTimerConfirm()
{
	if (TargetActor)
	{
		TargetActor->Confirm();
	}
}

void UGA_BossShadowAttack::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
	ABossShadowTargetActor* BossShadowTargetActor = Cast<ABossShadowTargetActor>(TargetActorData.TargetActor);
	if (!BossShadowTargetActor)
	{
		Super::OnTargetActorConfirm(TargetActorData);
	}

	BP_OnTargetActorConfirm(TargetActorData);

	GetAvatarActorFromActorInfo()->SetActorLocation(BossShadowTargetActor->GetActorLocation());
	GetAvatarActorFromActorInfo()->SetActorRotation(BossShadowTargetActor->GetActorRotation());

	if (UST_Base* BossST = BossController->GetStateTreeComponent())
	{
		BossST->SendStateTreeEvent(GAS_Tags::TAG_AI_StateTreeEvent_ExecuteShadowAttack, FConstStructView::Make(TargetActorData));
	}

	Super::OnTargetActorConfirm(TargetActorData);
}
