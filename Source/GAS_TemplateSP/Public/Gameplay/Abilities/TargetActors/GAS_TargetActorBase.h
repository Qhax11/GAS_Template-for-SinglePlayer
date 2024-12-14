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

	UPROPERTY(BlueprintReadOnly, Category = "TargetActorData")
	AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly, Category = "TargetActorData")
	UObject* OptionalObject;

	FGAS_TargetActorData() = default;

	FGAS_TargetActorData(AActor* InTargetActor, UObject* InOptionalObject)
		: TargetActor(InTargetActor),
		OptionalObject(OptionalObject)
	{}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorConfirm, const FGAS_TargetActorData&, TargetActorData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorCancel, const FGAS_TargetActorData&, TargetActorData);


UCLASS()
class GAS_TEMPLATESP_API AGAS_TargetActorBase : public AActor
{
	GENERATED_BODY()
	
public:


	virtual void Confirm();

	virtual void Cancel();

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorConfirm OnConfirm;

	UPROPERTY(BlueprintAssignable)
	FOnTargetActorCancel OnCancel;
};
