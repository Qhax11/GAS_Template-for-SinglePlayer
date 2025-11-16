// Qhax's GAS Template for SinglePlayer

#pragma once

#include "Components/ActorComponent.h"
#include "Gameplay/Components/GameplayTag/AC_TagDelegates.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Tags/GAS_Tags.h"
#include "AbilitySystemComponent.h"
#include "AC_TagListenerBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEMPLATESP_API UAC_TagListenerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_TagListenerBase();

protected:
	virtual void BeginPlay() override;

	virtual void BindTagDelegates();

	UPROPERTY()
	class AGAS_CharacterBase* OwnerCharacter;

	UPROPERTY()
	UAbilitySystemComponent* OwnerCharacterASC;

	UPROPERTY()
	UCharacterMovementComponent* OwnerCharacterMoveComp;

	UPROPERTY()
	UAC_TagDelegates* OwnerTagDelegatesComp;
};
