// Qhax's GAS Template for 2D SinglePlayer

#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/Components/GAS_AbilitySystemComponent.h"
#include "GAS_CharacterBase.generated.h"


UCLASS()
class GAS_TEMPLATESP_API AGAS_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AGAS_CharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UGAS_AbilitySystemComponent* PaperCharacterASC;

protected:

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
	class UAC_TagDispatcher* TagDispatcherComponent;

private:
	class UAC_TagDelegates* TagDelegatesComponent;


public:

	void DisableMovement();

	void EnableMovement();

	void DisableCollision();

	void EnableCollision();

};
