// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Abilities/GA_TargetBase.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "Gameplay/Actors/Characters/Heroes/Components/AC_HeroControl.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UGA_TargetBase::UGA_TargetBase()
{
	ActivationOwnedTags.AddTag(GAS_Tags::TAG_Gameplay_State_AbilityTargeting);
}

void UGA_TargetBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UGA_TargetBase::StartEQSForTargetActorSpawnLocation()
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query Template is not set!"));
		return;
	}

	FEnvQueryRequest QueryRequest(EQSQueryTemplate, GetOwningActorFromActorInfo());

	QueryRequest.Execute(QueryRunMode, this, &UGA_TargetBase::OnTargetActorSpawnLocationQueryFinished);
}

void UGA_TargetBase::OnTargetActorSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query did not return any results."));
		SpawnAndSetupTargetActor();
		return;
	}

	FVector BestLocation = Result->GetItemAsLocation(0);

	SpawnAndSetupTargetActor(FRotator::ZeroRotator, BestLocation);
}

void UGA_TargetBase::SpawnAndSetupTargetActor(FRotator Rotation, FVector Location)
{
	if (UWorld* World = this->GetWorld())
	{
		if (TargetActorClass->IsValidLowLevelFast())
		{
			FTransform ActorTransform = FTransform(Rotation, Location);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
			TargetActor = World->SpawnActor<AGAS_TargetActorBase>(TargetActorClass, ActorTransform, SpawnParams);
		}

		if (TargetActor)
		{
			TargetActor->OnConfirm.AddDynamic(this, &UGA_TargetBase::OnTargetActorConfirm);
			TargetActor->OnCancel.AddDynamic(this, &UGA_TargetBase::OnTargetActorCancelled);
			TargetActor->OnInitialized.AddDynamic(this, &UGA_TargetBase::OnTargetActorInitialized);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor cannot spawned in: %s"), *GetName());
			EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
		}
	}
}

void UGA_TargetBase::OnTargetActorInitialized()
{
	// Logic will be implemented in subclasses if necessary.
}

void UGA_TargetBase::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
	TargetActor->DestroyTargetActor();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_TargetBase::OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData)
{
	BP_OnTargetActorCancelled(TargetActorData);
	TargetActor->DestroyTargetActor();
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
}



