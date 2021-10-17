using System.IO;
using System.Collections.Generic;
using Sharpmake;

[module: Sharpmake.Include("ion/ion.make.cs")]
[module: Sharpmake.Include("../LUMINARY/TOOLS/BeehivePlugins/luminary/luminary.make.cs")]

[Generate]
class BeehiveStudio : IonExe
{
    enum ExportPlugin
    {
        Tanglewood,
        Luminary
    }

    ExportPlugin exportPlugin = ExportPlugin.Luminary;

    public BeehiveStudio() : base("BeehiveStudio", "MDStudio")
    {
        AddTargets(Globals.IonTargetsDefault);
        SourceFilesExtensions.Add(".glsl");
    }

    protected override void ExcludeOutputFiles()
    {
        base.ExcludeOutputFiles();
        BuildShader.ConfigureShaders(this);
    }

    [Configure]
    public override void Configure(Project.Configuration conf, Target target)
    {
        base.Configure(conf, target);

        // ion::engine
        conf.AddPublicDependency<IonCore>(target);
        conf.AddPublicDependency<IonMaths>(target);
        conf.AddPublicDependency<IonRenderer>(target);
        conf.AddPublicDependency<IonShaders>(target, DependencySetting.DefaultWithoutLinking);
        conf.AddPublicDependency<IonGameKit>(target);
        conf.AddPublicDependency<IonBeehive>(target);
        conf.AddPublicDependency<IonResource>(target);

        // Dependencies
        conf.AddPublicDependency<Dependencies.LibWxWidgets>(target);
        conf.AddPublicDependency<Dependencies.LibSLZ>(target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/mdstudio/ui");
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/ion/renderer/imageformats");

        // Configure features
        switch (exportPlugin)
        {
            case ExportPlugin.Luminary:
            {
                conf.AddPublicDependency<Luminary>(target);

                conf.Defines.Add("BEEHIVE_PLUGIN_LUMINARY=1");
                conf.Defines.Add("BEEHIVE_FIXED_STAMP_MODE=1");
                conf.Defines.Add("BEEHIVE_LEAN_UI=1");
                conf.Defines.Add("BEEHIVE_GAMEOBJ_ORIGIN_CENTRE=1");

                // Requires Luminary checkout at ../luminary
                conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/../LUMINARY/TOOLS/BeehivePlugins");
                break;
            }
        }

        // Assets
        conf.TargetCopyFilesToSubDirectory.Add(new KeyValuePair<string, string>(@"[project.SharpmakeCsPath]/.build/projects/shaders/", @"[project.SharpmakeCsPath]/MDStudio/shaders"));
    }
}

[Generate]
class BeehiveStudioSolution : IonSolution
{
    public BeehiveStudioSolution() : base("BeehiveStudio")
    {
        AddTargets(Globals.IonTargetsDefault);
    }

    [Configure]
    public override void Configure(Solution.Configuration conf, Target target)
    {
        base.Configure(conf, target);

        // Application
        conf.AddProject<BeehiveStudio>(target);
    }
}

public static class BuildMain
{
    [Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<BeehiveStudioSolution>();
    }
}