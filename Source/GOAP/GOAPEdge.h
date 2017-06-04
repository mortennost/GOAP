#pragma once

#include "GOAPAction.h"
#include "GOAPEdge.generated.h"

struct FGOAPNode;

// GOAP Edge struct used in GOAPPlanner's search algorithm
USTRUCT()
struct GOAP_API FGOAPEdge
{
	GENERATED_BODY()

public:
	FGOAPEdge() {};
	~FGOAPEdge() {};

	TWeakObjectPtr<UGOAPAction> Action;
	FGOAPNode* Node = nullptr;
};