#pragma once

#include "EnvironmentQuery/EnvQuery.h"
#include "GOAPEQSJob.generated.h"

// Struct defining an EQS job that actions can send to the AIController to process
USTRUCT(BlueprintType)
struct GOAP_API FGOAPEQSJob
{
	GENERATED_BODY()

public:
	// The GOAP Action that sent this EQS Job
	UPROPERTY()
	TWeakObjectPtr<UGOAPAction> CallingAction;

	// The EQS query itself
	UPROPERTY()
	UEnvQuery* Query;

	// The Query RunMode type of this query job
	UPROPERTY()
	TEnumAsByte<EEnvQueryRunMode::Type> QueryRunMode;
};