#pragma once

#include "GOAPState.generated.h"

USTRUCT()
struct GOAP_API FGOAPState
{
	GENERATED_BODY()

public:

	FGOAPState() {};
	~FGOAPState() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<uint8, bool> State;

	FGOAPState& operator+(const FGOAPState& inState)
	{
		for (auto& stateElement : inState.State)
		{
			State.Add(stateElement.Key, stateElement.Value);
		}

		return *this;
	}

	// Checks if this full GOAPState Key/Value collection is satisfied
	bool FGOAPState::IsGOAPStateSatisfied(const FGOAPState& inState)
	{
		for (auto& stateElement : inState.State)
		{
			// Check if local State map contains given state
			if (State.Contains(stateElement.Key))
			{
				// Get corresponding for key value in local State map
				bool* localValue = State.Find(stateElement.Key);

				// State is not satisfied
				if (*localValue != stateElement.Value)
					return false;
			}
		}

		return true;
	}

	// Checks if a state key, and it's corresponding value, is satisfied
	bool FGOAPState::IsStateSatisfied(const uint8 stateKey, const bool stateValue)
	{
		// Check if local State map contains state with given key
		if (State.Contains(stateKey))
		{
			// Get corresponding value for key in local State map
			bool* localValue = State.Find(stateKey);

			// Values are equal, state is satisfied
			if (*localValue == stateValue)
				return true;
		}

		return false;
	}

	// Sets a state's value. Adds the state and corresponding value if it does not already exist
	void FGOAPState::SetState(const uint8 stateKey, const bool stateValue)
	{
		State.Add(stateKey, stateValue);
	}
};