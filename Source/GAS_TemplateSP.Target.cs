// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_TemplateSPTarget : TargetRules
{
	public GAS_TemplateSPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;  // ← V5 yerine Latest
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;  // ← Unreal5_7 yerine Latest
        ExtraModuleNames.Add("GAS_TemplateSP");
	}
}
