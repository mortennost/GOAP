#pragma once

#include "GOAPState.h"
#include "GOAPEdge.h"
#include "GOAPNode.generated.h"

// GOAP Node struct used in GOAPPlanner's search algorithm
USTRUCT()
struct GOAP_API FGOAPNode
{
	GENERATED_BODY()

public:
	FGOAPNode() {};
	~FGOAPNode() {};

	FGOAPState State;
	FGOAPEdge Parent;
};