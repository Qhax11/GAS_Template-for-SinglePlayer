// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class GAS_TemplateSP : ModuleRules
{
	public GAS_TemplateSP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
            "GameplayAbilities",
            "EnhancedInput",
            "GameplayTasks",
            "GameplayTags",
            "AIModule",
			"GameplayStateTreeModule",
            "StateTreeModule",
            "AnimGraphRuntime",
            "MotionWarping"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore" 
		});
	}
}
