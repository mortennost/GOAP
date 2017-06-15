// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GOAPEditor : ModuleRules
{
	public GOAPEditor(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",  "GOAP", "InputCore", "GameplayTasks", "AIModule", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "PropertyEditor", "EditorStyle", "UnrealEd", "GraphEditor", "BlueprintGraph" });
		
		PublicIncludePaths.AddRange(
			new string[] {
				"GOAPEditor"
			});
			
		PrivateIncludePaths.AddRange(
			new string[] {
				"GOAPEditor"
			});
	}
}
