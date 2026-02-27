using UnrealBuildTool;
using System.Collections.Generic;

public class F4EditorTarget : TargetRules
{
	public F4EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "F4" } );
	}
}
