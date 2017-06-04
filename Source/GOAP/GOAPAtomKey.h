#pragma once

#include "GOAPAtomKey.generated.h"

USTRUCT(BlueprintType)
struct GOAP_API FGOAPAtomKey
{
	GENERATED_BODY()

public:
	// Struct is required to be able to customize blueprint pins
	FGOAPAtomKey(uint8 inKey) : Key(inKey) {};
	FGOAPAtomKey() {};
	~FGOAPAtomKey() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Key;
};