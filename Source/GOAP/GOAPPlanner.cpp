#include "GOAP.h"
#include "GOAPPlanner.h"
#include "GOAPNode.h"
#include "GOAPAtom.h"
#include "GOAPAIController.h"
#include "GOAPAction.h"

UGOAPPlanner::UGOAPPlanner(const FObjectInitializer& objInitializer) : Super(objInitializer)
{

}

TArray<TWeakObjectPtr<UGOAPAction>> UGOAPPlanner::Plan(UObject* outer, const int32 maxNodes, const uint8 inState, const bool inValue, TArray<UGOAPAction*>* actions, FGOAPState* currentState, AGOAPAIController& controller)
{
	// Empty arrays before new plan is started
	_OpenNodes.Empty();
	_ClosedNodes.Empty();
	_ClosedNodes.Reserve(maxNodes);
	_GOAPPlan.Empty();

	// First build the graph, start building from the current state (WorldState)
	FGOAPNode startNode;
	startNode.State = *currentState;
	_OpenNodes.Push(startNode);

	while (_OpenNodes.Num() > 0)
	{
		_ClosedNodes.Push(_OpenNodes.Pop(false));

		FGOAPNode& workNode = _ClosedNodes[_ClosedNodes.Num() - 1];

		// Continue searching this branch only if inState is not already satisfied
		if (!workNode.State.IsStateSatisfied(inState, inValue))
		{
			// Get all valid actions for this state to check
			for (auto& action : controller.GetValidActionsForState(workNode.State))
			{
				// Make sure we aren't caught in an action loop by only allowing actions that are
				// not equal to the one currently processing to be pushed onto the action queue
				// E.g. EnterCover->ExitCover->EnterCover->ExitCover
				if (!workNode.Parent.Action.IsValid() || !workNode.Parent.Node->Parent.Action.IsValid() || action != workNode.Parent.Node->Parent.Action)
				{
					// Procedural precondition(s) NOT satisfied. This is not a valid action.
					if (!action->CheckProceduralPreconditions(&controller))
					{
						continue;
					}

					// The new node is the previous state plus the effects of the action that got us here
					FGOAPNode newNode;
					newNode.State = workNode.State;
					newNode.State = newNode.State + action->Effects_Internal;
					newNode.Parent.Action = action;
					newNode.Parent.Node = &workNode;
					_OpenNodes.Push(newNode);
				}
			}
		}

		if (_ClosedNodes.Num() >= maxNodes)
		{
			// We've run out of nodes, usually because of a circular route
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Planning failed. Circular routes??"));
			
			return _GOAPPlan;
		}
	}

	// Now to find any and all nodes that meet the target state
	TArray<FGOAPNode*> validNodes;
	for (auto& node : _ClosedNodes)
	{
		if (node.State.IsStateSatisfied(inState, inValue))
		{
			validNodes.Push(&node);
		}
	}

	// Compile the list of valid plans 
	TArray<TArray<TWeakObjectPtr<UGOAPAction>>> validPlans;
	for (auto& node : validNodes)
	{
		FGOAPNode* currentNode = node;

		TArray<TWeakObjectPtr<UGOAPAction>> planCandidate;
		while (currentNode->Parent.Action.IsValid())
		{
			planCandidate.Push(currentNode->Parent.Action);
			currentNode = currentNode->Parent.Node;
		}

		validPlans.Push(planCandidate);
	}

	// Pick the plan with the lowest cost
	int32 shortestPlanCost = INT_MAX;
	int16 index = 0;
	int16 shortestPlanIndex = -1;

	for (auto& plan : validPlans)
	{
		int32 costOfPlan = 0;
		for (auto& planAction : plan)
		{
			costOfPlan += planAction->Cost;
		}

		if (costOfPlan < shortestPlanCost)
		{
			shortestPlanCost = costOfPlan;
			shortestPlanIndex = index;
		}

		index++;
	}

	if (shortestPlanIndex > -1)
	{
		// Need to reverse the plan
		while (validPlans[shortestPlanIndex].Num() > 0)
		{
			_GOAPPlan.Push(validPlans[shortestPlanIndex].Pop());
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Planning complete: Nodes '%i' - Valid plans '%i' - Optimal plan cost '%i'"),
																_ClosedNodes.Num(),
																validPlans.Num(),
																shortestPlanCost));

	FString planString;
	for (auto& action : _GOAPPlan)
	{
		planString += action->ActionDescription + TEXT(":");
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, planString);

	return _GOAPPlan;
}
