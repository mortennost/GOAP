#pragma once

#include "GOAPAtom.generated.h"


USTRUCT(BlueprintType)
struct GOAP_API FGOAPAtom
{
	GENERATED_BODY()

public:
	FGOAPAtom(uint8 inKey, bool inValue) : Key(inKey), Value(inValue) {};
	FGOAPAtom() {};
	~FGOAPAtom() {};

	bool operator==(const FGOAPAtom& inAtom)
	{
		return inAtom.Key == Key && inAtom.Value == Value;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Value;
};