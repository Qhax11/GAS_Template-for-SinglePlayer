// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"
#include "Gameplay/Abilities/TargetActors/Shadows/BossShadowTargetActor.h"
#include "Gameplay/Actors/Characters/Enemies/Components/AC_EnemyMovementManager.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
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

	ExecuteMovementChain();
}

void UGA_BossShadowAttack::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
	FVector ShadowSpawnLocation = Location;

	if (AActor* Target = BossController->GetTargetActor()) 
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

void UGA_BossShadowAttack::ExecuteMovementChain()
{
	if (UAC_EnemyMovementManager* EnemyMovementManagerComp = BossCharacter->GetEnemyMovementManagerComponent()) 
	{
		//EnemyMovementManagerComp->StartMovementChain(GetClass());
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
		UE_LOG(LogTemp, Warning, TEXT("BossShadowTargetActor is null in: %s"), *GetName());
		OnTargetActorCancelled(TargetActorData);
		return;
	}

	if (!TargetActorData.AbilityClass || !TargetActorData.AbilityCDO) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityClass or AbilityCDO is null in: %s"), *GetName());
		OnTargetActorCancelled(TargetActorData);
		return;
	}

	// If we're within range, proceed to perform the attack, it's mean shadow attack successful
	if (GetTargetDistance(BossShadowTargetActor) < TargetActorData.AbilityCDO->MaxRange)
	{
		// Ensure all shadow attacks start from Section2 of the montage
		TargetActorData.AbilityCDO->SectionName = FName("Section2");

		GetAvatarActorFromActorInfo()->SetActorLocation(BossShadowTargetActor->GetActorLocation());
		GetAvatarActorFromActorInfo()->SetActorRotation(BossShadowTargetActor->GetActorRotation());

		if (UAC_EnemyMovementManager* EnemyMovementManagerComp = BossCharacter->GetEnemyMovementManagerComponent())
		{
			EnemyMovementManagerComp->StopChain();
		}

		OnBossShadowAttackCompleted.Broadcast(TargetActorData);

		Super::OnTargetActorConfirm(TargetActorData);
		return;
	}
	else
	{
		OnTargetActorCancelled(TargetActorData);
	}
}

float UGA_BossShadowAttack::GetTargetDistance(AActor* ShadowTargetActor)
{
	if (!BossController || !BossController->GetTargetHero() || !ShadowTargetActor)
	{
		return -1.f;
	}

	FVector MyLocation = ShadowTargetActor->GetActorLocation();
	FVector TargetLocation = BossController->GetTargetHero()->GetActorLocation();

	return FVector::Dist(MyLocation, TargetLocation);
}

void UGA_BossShadowAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAC_EnemyMovementManager* EnemyMovementManagerComp = BossCharacter->GetEnemyMovementManagerComponent())
	{
		EnemyMovementManagerComp->StopChain();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
