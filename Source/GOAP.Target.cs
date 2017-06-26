// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GOAPTarget : TargetRules
{
	public GOAPTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;

	    ExtraModuleNames.Add("GOAP");

	    if (UEBuildConfiguration.bBuildEditor)
	    {
	        ExtraModuleNames.Add("GOAPEditor");
	    }
    }
}
