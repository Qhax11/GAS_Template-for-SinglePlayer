// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_TemplateSPEditorTarget : TargetRules
{
	public GAS_TemplateSPEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;  // ← V5 yerine Latest
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;  // ← Unreal5_7 yerine Latest
        ExtraModuleNames.Add("GAS_TemplateSP");
	}
}
