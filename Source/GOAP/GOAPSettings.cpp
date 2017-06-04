#pragma once

#include "GOAP.h"
#include "GOAPSettings.h"

TArray<FString> UGOAPSettings::GetStateSettings()
{
	return WorldStates;
}

TSharedPtr<FString> UGOAPSettings::GetStringForByte(const uint8 inByte)
{
	TSharedPtr<FString> result;

	// Make sure given byte index is within the range of the WorldStates array
	if (inByte < WorldStates.Num())
	{
		result = MakeShareable<FString>(new FString(WorldStates[inByte]));
	}
	else
	{
		// Byte index not within array range, return "Invalid" result
		result = MakeShareable<FString>(new FString(TEXT("Invalid")));
	}

	return result;
}

uint8 UGOAPSettings::GetByteKey(TSharedPtr<FString> inString)
{
	for (uint8 i = 0; i < WorldStates.Num(); i++)
	{
		// Return byte index in WorldStates array where corresponding string name was found
		//	 - FString->Compare() == 0 ----> EQUAL
		if (inString->Compare(WorldStates[i]) == 0)
		{
			return i;
		}
	}

	return 0;
}
