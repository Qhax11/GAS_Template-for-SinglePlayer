// Qhax's GAS Template for SinglePlayer


#include "Gameplay/Actors/Characters/Heroes/Components/AC_AbilityInputBinding.h"
#include "Gameplay/Actors/Characters/Heroes/GAS_HeroBase.h"
#include "AbilitySystemGlobals.h"

namespace EnhancedInputAbilitySystem_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

UAC_AbilityInputBinding::UAC_AbilityInputBinding()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAC_AbilityInputBinding::BeginPlay()
{
	Super::BeginPlay();

	if (!HeroBase || !HeroASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroBase or HeroASC is null in: %s)"), *GetName());
		return;
	}

	if (!HeroBase->InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is null in: %s)"), *GetName());
		return;
	}

	InputComponent = CastChecked<UEnhancedInputComponent>(HeroBase->InputComponent);
	check(InputComponent);
}

void UAC_AbilityInputBinding::SetInputBinding(UInputAction* InputAction, const FGameplayAbilitySpecHandle& AbilityHandle)
{
	using namespace EnhancedInputAbilitySystem_Impl;

	FGameplayAbilitySpec* BindingAbility = HeroASC->FindAbilitySpecFromHandle(AbilityHandle);

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding)
	{
		FGameplayAbilitySpec* OldBoundAbility = HeroASC->FindAbilitySpecFromHandle(AbilityInputBinding->BoundAbilitiesStack.Top());
		if (OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = InvalidInputID;
		}
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = GetNextInputID();
	}

	if (BindingAbility)
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilityHandle);
	TryBindAbilityInput(InputAction, *AbilityInputBinding);
}

void UAC_AbilityInputBinding::OnAbilityInputPressed(UInputAction* InputAction)
{
	using namespace EnhancedInputAbilitySystem_Impl;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
	{
		HeroASC->AbilityLocalInputPressed(FoundBinding->InputID);
	}
}

void UAC_AbilityInputBinding::OnAbilityInputReleased(UInputAction* InputAction)
{
	using namespace EnhancedInputAbilitySystem_Impl;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
	{
		HeroASC->AbilityLocalInputReleased(FoundBinding->InputID);
	}
}

void UAC_AbilityInputBinding::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding)
{
	if (InputComponent)
	{
		// Pressed event
		if (AbilityInputBinding.OnPressedHandle == 0)
		{
			AbilityInputBinding.OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAC_AbilityInputBinding::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0)
		{
			AbilityInputBinding.OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAC_AbilityInputBinding::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}
