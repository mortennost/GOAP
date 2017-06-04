#pragma once

#include "GOAPAtom.h"
#include "GOAPStateEditor.generated.h"

// We have a different struct for configuration purposes
//	 so that we can specify our custom configuration panels in editor
USTRUCT()
struct GOAP_API FGOAPStateEditor
{
	GENERATED_BODY()

public:
	FGOAPStateEditor() {};
	~FGOAPStateEditor() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGOAPAtom> State;
};