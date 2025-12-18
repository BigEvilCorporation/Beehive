using System.IO;
using System.Collections.Generic;
using Sharpmake;

[module: Sharpmake.Include("ion/ion.make.cs")]

[Generate]
class BeehiveStudio : IonExe
{
    public BeehiveStudio() : base("BeehiveStudio", "MDStudio")
    {
        AddTargets(Globals.IonTargetsTool);
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

        // Engine assets
        var shaders = Directory.GetFiles("ion/shaders", "*.ion.shader");
        foreach(var shader in shaders)
        {
            conf.TargetCopyFilesToSubDirectory.Add(new KeyValuePair<string, string>(shader, @"[project.SharpmakeCsPath]/MDStudio/shaders"));
        }

        // Working dir
        conf.VcxprojUserFile = new Project.Configuration.VcxprojUserFileSettings();
        conf.VcxprojUserFile.LocalDebuggerWorkingDirectory = @"[project.SharpmakeCsPath]/MDStudio";
    }
}

[Generate]
class BeehiveStudioSolution : IonSolution
{
    public BeehiveStudioSolution() : base("BeehiveStudio")
    {
        AddTargets(Globals.IonTargetsTool);
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