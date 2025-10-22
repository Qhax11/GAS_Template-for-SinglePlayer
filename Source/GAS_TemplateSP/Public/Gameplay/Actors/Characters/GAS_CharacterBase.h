// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Gameplay/Actors/Weapons/WeaponBase.h"
#include "GAS_CharacterBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGAS_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGAS_CharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE class UAC_AbilitySet* GetAbilitySetComponent() const { return AbilitySetComponent; }

	FORCEINLINE class UAC_TagDispatcher* GetTagDispatcherComponent() const { return TagDispatcherComponent; }

	FORCEINLINE class UAC_TagDelegates* GetTagDelegatesComponent() const { return TagDelegatesComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AWeaponBase* GetWeapon() const { return Cast<AWeaponBase>(WeaponChildComponent->GetChildActor()); }

	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComp; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UGAS_AbilitySystemComponent* CharacterASC;

	/* Components */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_AbilitySet* AbilitySetComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_Team* TeamComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_AttributesListenerBase* AttributesListenerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_GameplayData* GameplayDataComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_PostureHandler* PostureHandlerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_TagListenerBase* TagListenerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterBase|Components")
	class UAC_TagDispatcher* TagDispatcherComponent;

	class UAC_TagDelegates* TagDelegatesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Components")
	class UChildActorComponent* WeaponChildComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Components")
	class UAC_FootstepBase* FootstepComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Components")
	UMotionWarpingComponent* MotionWarpingComp;

public:
	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;

public:
	// Commonly used in death state logic
	void DisableMovement();

	void EnableMovement();

	void DisableCollision(ECollisionEnabled::Type NewType);

	void EnableCollision();

	void DisableMesh();

	void EnableMesh();

public:
	// Generic
	void AddGameplayTagIfNotExist(FGameplayTag GameplayTag);

	void RemoveGameplayTagIfExist(FGameplayTag GameplayTag);

	void AddGameplayTagsIfNotExist(FGameplayTagContainer& GameplayTags);

	void RemoveGameplayTagsIfExist(FGameplayTagContainer& GameplayTags);

private:
	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMode, uint8 PreviousCustomMode);
	
};
