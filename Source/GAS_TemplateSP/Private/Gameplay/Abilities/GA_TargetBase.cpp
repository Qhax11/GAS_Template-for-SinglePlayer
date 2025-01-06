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
	UGameplayAbility::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (BindInputForConfirmAndCancel())
	{
		if (bActorWillSpawnWithEQS) 
		{
			StartEQSForTargetActorSpawnLocation();
		}
		else
		{
			SpawnAndSetupTargetActor();
		}
	}
}

bool UGA_TargetBase::BindInputForConfirmAndCancel()
{
	AGAS_HeroBase* HeroBase = Cast<AGAS_HeroBase>(GetAvatarActorFromActorInfo());
	if (!HeroBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase is null in %s. Cannot bind input for targeting because the Avatar Actor is not a valid hero."), *GetName());
		return false;
	}

	if (UAC_HeroControl* GetHeroControlComponent = HeroBase->GetHeroControlComponent())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = GetHeroControlComponent->GetEnhancedInputComponent())
		{
			if (GetHeroControlComponent->IA_ConfirmTarget && GetHeroControlComponent->IA_CancelTarget)
			{
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_ConfirmTarget, ETriggerEvent::Triggered, this, &UGA_TargetBase::ConfirmTargetingFromInput);
				EnhancedInputComponent->BindAction(GetHeroControlComponent->IA_CancelTarget, ETriggerEvent::Triggered, this, &UGA_TargetBase::CancelAbilityFromInput);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Input actions are null in: %s"), *GetName());
			}
		}
	}

	return false;
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
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor cannot spawned in: %s"), *GetName());
			EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
		}
	}
}

void UGA_TargetBase::StartEQSForTargetActorSpawnLocation()
{
	if (!EQSQueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Query Template is not set!"));
		return;
	}

	FEnvQueryRequest QueryRequest(EQSQueryTemplate, GetOwningActorFromActorInfo());

	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UGA_TargetBase::OnTargetActorSpawnLocationQueryFinished);
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

void UGA_TargetBase::OnTargetActorConfirm(const FGAS_TargetActorData& TargetActorData)
{
	BP_OnTargetActorConfirm(TargetActorData);
	TargetActor->Destroy();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_TargetBase::OnTargetActorCancelled(const FGAS_TargetActorData& TargetActorData)
{
	BP_OnTargetActorCancelled(TargetActorData);
	TargetActor->Destroy();
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
}

void UGA_TargetBase::ConfirmTargetingFromInput()
{
	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	StartupEffects();

	if (TargetActor)
	{
		TargetActor->Confirm();
	}
}

void UGA_TargetBase::CancelAbilityFromInput()
{
	if (TargetActor)
	{
		TargetActor->Cancel();
	}
}


