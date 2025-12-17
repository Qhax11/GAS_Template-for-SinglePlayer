// Qhax's GAS Template for SinglePlayer

#pragma once

#include "UObject/NoExportTypes.h"
#include "DecisionOptionData.generated.h"

/**
 * UDecisionOptionData
 *
 * Abstract base class representing a single selectable option in the AI decision system.
 *
 * This class defines the **common decision-evaluation pipeline** shared by all AI choices
 * (attacks, movements, reactions, etc.), without knowing any domain-specific details.
 *
 * Each option is evaluated in three sequential stages:
 *
 * 1) IsEnable
 *    - Hard, deterministic gating.
 *    - Answers: "Is this option even allowed right now?"
 *    - Used for absolute constraints such as state locks, cooldowns, distance, or timing.
 *
 * 2) PassesChance
 *    - Probabilistic commitment check.
 *    - Answers: "Even if allowed, does the AI commit to this option at this moment?"
 *    - Introduces controlled uncertainty to avoid perfectly deterministic behavior.
 *
 * 3) GetScore
 *    - Deterministic prioritization among the remaining valid options.
 *    - Answers: "Which allowed option is the best choice right now?"
 *
 * IMPORTANT DESIGN NOTES:
 * - This class is **domain-agnostic**: it does not know what kind of option it represents.
 * - Context and debug data are intentionally passed as `void*` to avoid coupling this base
 *   class to any specific gameplay domain (reaction, attack, movement, etc.).
 * - Concrete subclasses (e.g. reaction, attack, movement data) are responsible for:
 *     - Defining the actual context type
 *     - Interpreting the debug data
 *     - Implementing domain-specific logic
 *
 * Conceptual model:
 *   Enable -> Chance -> Score -> Selection
 *
 * This base exists to enforce a consistent mental model and evaluation flow across all
 * AI decision systems while keeping domain logic cleanly separated and extensible.
 */

UCLASS(Blueprintable, DefaultToInstanced, EditInLineNew, Abstract)
class GAS_TEMPLATESP_API UDecisionOptionData : public UObject
{
	GENERATED_BODY()

public:
	// Hard gate
	virtual bool IsEnable(const void* Context, void* OutDebug = nullptr) const
	{
		return true;
	}

	// Probabilistic gate
	virtual bool PassesChance(const void* Context, void* OutDebug = nullptr) const
	{
		return true;
	}

	// Deterministic priority
	virtual float GetScore(const void* Context, void* OutDebug = nullptr) const
	{
		return 0.f;
	}
	
};
