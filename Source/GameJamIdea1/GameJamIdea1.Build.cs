// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJamIdea1 : ModuleRules
{
	public GameJamIdea1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayAbilities", "GameplayTags", "GameplayTasks", "UMG" });
	}
}
