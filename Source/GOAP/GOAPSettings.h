#pragma once

#include "GOAPSettings.generated.h"

// Implements the settings for GOAP (Saved in GOAP.ini file)
UCLASS(config=GOAP, BlueprintType)
class GOAP_API UGOAPSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category=State)
	TArray<FString> WorldStates;

	UFUNCTION(BlueprintPure, Category = "GOAP")
	TArray<FString> GetStateSettings();

	// Returns the corresponding string value from a byte (uint8) index in the WorldStates array
	TSharedPtr<FString> GetStringForByte(const uint8 inByte);

	// Returns the corresponding byte (uint8) value from the WorldStates array based on the given string
	uint8 GetByteKey(TSharedPtr<FString> inString);
};