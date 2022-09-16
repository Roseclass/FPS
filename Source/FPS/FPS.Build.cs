// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPS : ModuleRules
{
	public FPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore" ,
			"UMG" ,
			"Slate" ,
			"SlateCore" ,
			"GameplayTasks",
			"Niagara",
			"ProceduralMeshComponent",
			"AIModule"
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "NinjaCharacter" });
		PublicIncludePaths.Add(ModuleDirectory);
	}
}
