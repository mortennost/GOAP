// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GOAPEditorTarget : TargetRules
{
	public GOAPEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
        
	    ExtraModuleNames.Add("GOAP");
	    ExtraModuleNames.Add("GOAPEditor");
    }
}
