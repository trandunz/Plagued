// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Plagued : ModuleRules
{
	public Plagued(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Niagara"});
		
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem"});
		
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks"});
		
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
	}
}
