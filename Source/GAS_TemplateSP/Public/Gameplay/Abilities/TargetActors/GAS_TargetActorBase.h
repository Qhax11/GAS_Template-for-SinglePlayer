// Qhax's GAS Template for SinglePlayer

#pragma once

#include "GameFramework/Actor.h"
#include "GAS_TargetActorBase.generated.h"

/**
 * A custom implementation of an ability target system for handling actor targets.
 *
 * The default GAS (Gameplay Ability System) target actor and task system caused issues in my project.
 * Due to limitations and inconsistencies when using GAS's built-in target actor with tasks,
 * I developed my own actor targeting system to have more control and avoid these conflicts.
 * This custom target actor system allows for smoother interactions with abilities and tasks,
 * ensuring better performance and fewer errors when managing targets in gameplay.
 */


USTRUCT(BlueprintType)
struct FGAS_TargetActorData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActorData")
	TSubclassOf<class UGA_MeleeAttackBase> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActorData")
	class UGA_MeleeAttackBase* AbilityCDO;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetActorData")
	AActor* TargetActor;

	FGAS_TargetActorData() = default;

	FGAS_TargetActorData(TSubclassOf<class UGA_MeleeAttackBase> InAbilityClass, class UGA_MeleeAttackBase* InAbilityCDO, AActor* InTargetActor)
		: AbilityClass(InAbilityClass),
		AbilityCDO(InAbilityCDO),
		TargetActor(InTargetActor)
	{}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorConfirm, const FGAS_TargetActorData&, TargetActorData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorCancel, const FGAS_TargetActorData&, TargetActorData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetActorInitialized);

UCLASS()
class GAS_TEMPLATESP_API AGAS_TargetActorBase : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void Confirm();

	UFUNCTION(BlueprintCallable)
	virtual void Cancel();

	virtual void DestroyTargetActor();

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorConfirm OnConfirm;

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorCancel OnCancel;

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorInitialized OnInitialized;

	UFUNCTION(BlueprintCallable)
	void BrodcastInitalized();

protected:
	// Prevents overlap logic from running during actor destruction
	bool bBeingDestroyed = false;

};
