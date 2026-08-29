using UnrealBuildTool;

public class SmartCityEditorTarget : TargetRules
{
    public SmartCityEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("SmartCity");
    }
}
