// Qhax's GAS Template for SinglePlayer


#include "Gameplay/AI/States/Boss/Boss_State_Attack.h"
#include "Gameplay/Abilities/Enemy/Boss/GA_BossShadowAttack.h"
#include "Gameplay/AI/BehaviorDecision/DataTypes/Attack/AttackDataBase.h"

void UBoss_State_Attack::OnEnter(TSharedPtr<FStatePayloadBase> EnterPayload)
{
	Super::OnEnter(EnterPayload);
}

bool UBoss_State_Attack::SelectAndMakeAttack(TSharedPtr<FAttackStatePayload> AttackStatePayload)
{
	TSubclassOf<UGAS_GameplayAbilityBase> SelectedAttackClass = AttackStatePayload->AttackData->AbilityClass;
	if (!SelectedAttackClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedAttackClass is null in: %s"), *GetName());
		return false;
	}

	if (SelectedAttackClass->IsChildOf(UGA_BossShadowAttack::StaticClass()))
	{
		MakeShadowAttack();
		return true;
	}
	
	if (Super::SelectAndMakeAttack(AttackStatePayload))
	{
		return true;
	}

	return false;
}

void UBoss_State_Attack::MakeShadowAttack()
{
	/*
	UGAS_GameplayAbilityBase* ActivatedAbility = EnemyASC->TryActivateAbilityByClassAndReturnInstance(SelectedAttackClass);

	UGA_BossShadowAttack* ShadowAttack = Cast<UGA_BossShadowAttack>(ActivatedAbility);
	if (ShadowAttack)
	{
		if (!ShadowAttack->OnBossShadowAttackCompleted.IsAlreadyBound(this, &UBoss_State_Attack::ExecuteShadowAttack))
		{
			ShadowAttack->OnBossShadowAttackCompleted.AddDynamic(this, &UBoss_State_Attack::ExecuteShadowAttack);
		}

		ShadowAttack->OnAbilityEnded.RemoveAll(this);
		ShadowAttack->OnAbilityEnded.AddUObject(this, &UBoss_State_Attack::OnShadowAttackAbilityEnded);

		LastUsedShadowAttack = ShadowAttack;
	}
	*/
}

void UBoss_State_Attack::ExecuteShadowAttack(const FGAS_TargetActorData& ShadowActorData)
{
	/*
	if (ShadowActorData.AbilityClass)
	{
		EnemyASC->TryActivateAbilityByClass(ShadowActorData.AbilityClass);

	}
	*/
}

void UBoss_State_Attack::OnShadowAttackAbilityEnded(const FCustomAbilityEndedData& ShadowAttackAbilityEndedData)
{
	// If it dosen't cancelled it's mean executed and we keep listening from executed side
	if (ShadowAttackAbilityEndedData.bWasCancelled)
	{
		ExitRequest("OnShadowAttackAbilityEnded");
	}
}

void UBoss_State_Attack::OnExit_Implementation()
{
	Super::OnExit_Implementation();

	if (LastUsedShadowAttack)
	{
		LastUsedShadowAttack->OnAbilityEnded.RemoveAll(this);
		LastUsedShadowAttack = nullptr;
	}
}
