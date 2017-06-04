#pragma once

#include "GOAPAction.h"
#include "GOAPNode.h"
#include "GOAPPlanner.generated.h"

class AGOAPAIController;

// The actual GOAP Planner
UCLASS()
class GOAP_API UGOAPPlanner : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<FGOAPNode> _OpenNodes;

	UPROPERTY()
	TArray<FGOAPNode> _ClosedNodes;

	UPROPERTY()
	TArray<TWeakObjectPtr<UGOAPAction>> _GOAPPlan;

public:
	UGOAPPlanner(const FObjectInitializer& objInitializer);

	// Form a plan to satisfy the specified target state
	//	 Uses a very basic forward search from current state right now.
	//	 Should be rewritten to utilize a reverse A* search from desired goal to current state (current WorldState)
	TArray<TWeakObjectPtr<UGOAPAction>> Plan
	(
		UObject* outer, 
		const int32 maxNodes, 
		const uint8 state, 
		const bool value,
		TArray<UGOAPAction*>* actions, 
		FGOAPState* currentState, 
		AGOAPAIController& controller
	);
};