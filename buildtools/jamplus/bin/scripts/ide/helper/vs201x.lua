-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
local VisualStudio201xProjectMetaTable = {  __index = VisualStudio201xProjectMetaTable  }

local function GetMapPlatformToVSPlatform(platformName)
	return MapPlatformToVSPlatform and MapPlatformToVSPlatform[platformName] or platformName
end

local function GetMapConfigToVSConfig(configName)
	return MapConfigToVSConfig and MapConfigToVSConfig[configName] or configName
end

local function RealVSPlatform(platform)
	if VSNativePlatforms  and  VSNativePlatforms[platform] then
		return MapPlatformToVSPlatform[platform]
	end

	return "Win32"
end

local function RealVSConfig(platform, config)
	local realConfig = GetMapConfigToVSConfig(config)
	if VSNativePlatforms  and  VSNativePlatforms[platform] then
		return realConfig
	end

	return GetMapPlatformToVSPlatform(platform) .. ' ' .. realConfig
end

function VisualStudio201xProjectMetaTable:Write(outputPath, commandLines)
	local filename = outputPath .. self.ProjectName .. '.vcxproj'

	local info = ProjectExportInfo[self.ProjectName]
	if not info then
		info = { Name = self.ProjectName, Filename = filename, Uuid = '{' .. uuid.new():upper() .. '}' }
		ProjectExportInfo[self.ProjectName] = info
	end

	local project = Projects[self.ProjectName]

	-- Write header.
	table.insert(self.Contents, expand([[
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
]]))

	-- Write Configurations header
	table.insert(self.Contents, [[
  <ItemGroup Label="ProjectConfigurations">
]])

	for platformName in ivalues(Config.Platforms) do
		for configName in ivalues(Config.Configurations) do
			local configInfo =
			{
				VSPlatform = RealVSPlatform(platformName),
				VSConfig = RealVSConfig(platformName, configName),
			}
			table.insert(self.Contents, expand([==[
    <ProjectConfiguration Include="$(VSConfig)|$(VSPlatform)">
      <Configuration>$(VSConfig)</Configuration>
      <Platform>$(VSPlatform)</Platform>
    </ProjectConfiguration>
]==], configInfo, info, _G))
		end
	end

	table.insert(self.Contents, [[
  </ItemGroup>
]])

	-- Write Globals
	table.insert(self.Contents, expand([[
  <PropertyGroup Label="Globals">
    <ProjectGUID>$(Uuid)</ProjectGUID>
    <TargetFrameworkVersion>v4.0</TargetFrameworkVersion>
    <Keyword>MakeFileProj</Keyword>
    <ProjectName>$(Name)</ProjectName>
  </PropertyGroup>
]], info))

	table.insert(self.Contents, [[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
]])

	-- Write Configurations.
	for platformName in ivalues(Config.Platforms) do
		for configName in ivalues(Config.Configurations) do
			local jamCommandLine = os.path.escape(os.path.make_backslash(jamScript)) .. ' ' ..
					os.path.escape('-C' .. destinationRootPath) .. ' ' ..
					'-sPLATFORM=' .. platformName .. ' ' ..
					'-sCONFIG=' .. configName

			local configInfo =
			{
				Platform = platformName,
				Config = configName,
				VSPlatform = RealVSPlatform(platformName),
				VSConfig = RealVSConfig(platformName, configName),
				Defines = '',
				Includes = '',
				Output = '',
			}

			if project and project.Name and project.Name ~= '!BuildWorkspace' and project.Name ~= '!UpdateWorkspace' then
				if project.Defines and project.Defines[platformName] and project.Defines[platformName][configName] then
					configInfo.Defines = table.concat(project.Defines[platformName][configName], ';'):gsub('"', '\\&quot;')
				end
				if project.IncludePaths and project.IncludePaths[platformName] and project.IncludePaths[platformName][configName] then
					configInfo.Includes = table.concat(project.IncludePaths[platformName][configName], ';')
				end
				if project.OutputPaths and project.OutputPaths[platformName] and project.OutputPaths[platformName][configName] then
					configInfo.Output = project.OutputPaths[platformName][configName] .. project.OutputNames[platformName][configName]
				end
				configInfo.BuildCommandLine = jamCommandLine .. ' ' .. self.ProjectName
				configInfo.RebuildCommandLine = jamCommandLine .. ' -a ' .. self.ProjectName
				configInfo.CleanCommandLine = jamCommandLine .. ' clean:' .. self.ProjectName
			else
				configInfo.BuildCommandLine = project.BuildCommandLine and project.BuildCommandLine[1] or jamCommandLine
				configInfo.RebuildCommandLine = project.RebuildCommandLine and project.RebuildCommandLine[1] or (jamCommandLine .. ' -a')
				configInfo.CleanCommandLine = project.CleanCommandLine and project.CleanCommandLine[1] or (jamCommandLine .. ' clean')
			end

			configInfo.BuildCommandLine = configInfo.BuildCommandLine:gsub('<', '&lt;'):gsub('>', '&gt;')
			configInfo.RebuildCommandLine = configInfo.RebuildCommandLine:gsub('<', '&lt;'):gsub('>', '&gt;')
			configInfo.CleanCommandLine = configInfo.CleanCommandLine:gsub('<', '&lt;'):gsub('>', '&gt;')

			table.insert(self.Contents, expand([==[
  <PropertyGroup Condition="'$$(Configuration)|$$(Platform)'=='$(VSConfig)|$(VSPlatform)'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <BuildLogFile>$(destinationRootPath:gsub('/', '\\'))$(Platform)-$(Config)/$$(MSBuildProjectName).log</BuildLogFile>
    <NMakeBuildCommandLine>$(BuildCommandLine)</NMakeBuildCommandLine>
    <NMakeOutput>$(Output)</NMakeOutput>
    <NMakeCleanCommandLine>$(CleanCommandLine)</NMakeCleanCommandLine>
    <NMakeReBuildCommandLine>$(RebuildCommandLine)</NMakeReBuildCommandLine>
    <NMakePreprocessorDefinitions>$(Defines)</NMakePreprocessorDefinitions>
    <NMakeIncludeSearchPath>$(Includes)</NMakeIncludeSearchPath>
  </PropertyGroup>
]==], configInfo, info, _G))
		end
	end

	-- Write Files.
	table.insert(self.Contents, [[
  <ItemGroup>
]])
	self:_WriteFilesFlat(project.SourcesTree)
	table.insert(self.Contents, [[
  </ItemGroup>
]])

	table.insert(self.Contents, [[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
]])

	-- Write footer.
	table.insert(self.Contents, [[
</Project>
]])
	self.Contents = table.concat(self.Contents):gsub('\r\n', '\n'):gsub('\n', '\r\n')

	WriteFileIfModified(filename, self.Contents)

	---------------------------------------------------------------------------
	---------------------------------------------------------------------------
	-- Write the .vcxproj.filters file.
	---------------------------------------------------------------------------
	---------------------------------------------------------------------------
	local filename = outputPath .. self.ProjectName .. '.vcxproj.filters'
	self.Contents = {}

	-- Write header.
	table.insert(self.Contents, [[
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
]])

	-- Write folders.
	table.insert(self.Contents, [[
  <ItemGroup>
]])

	if project then
		self:_WriteFolders(project.SourcesTree, '')
	end

	table.insert(self.Contents, [[
  </ItemGroup>
]])

	-- Write Files.
	table.insert(self.Contents, [[
  <ItemGroup>
]])

	if project then
		self:_WriteFiles(project.SourcesTree, '')
	end

	table.insert(self.Contents, [[
  </ItemGroup>
]])

	-- Write footer.
	table.insert(self.Contents, [[
</Project>
]])

	self.Contents = table.concat(self.Contents):gsub('\r\n', '\n'):gsub('\n', '\r\n')

	WriteFileIfModified(filename, self.Contents)
end

function VisualStudio201xProject(projectName, options)
	return setmetatable(
		{
			Contents = {},
			ProjectName = projectName,
			Options = options,
		}, { __index = VisualStudio201xProjectMetaTable }
	)
end


function VisualStudio201xProjectMetaTable:_WriteFolders(folder, inFilter)
	for entry in ivalues(folder) do
		if type(entry) == 'table' then
			local filter = inFilter
			if filter ~= '' then filter = filter .. '\\' end
			filter = filter .. entry.folder
			self:_WriteFolders(entry, filter)
			table.insert(self.Contents, [[
    <Filter Include="]] .. filter .. [[">
    </Filter>
]])
		end
	end
end


function VisualStudio201xProjectMetaTable:_WriteFiles(folder, inFilter)
	for entry in ivalues(folder) do
		if type(entry) == 'table' then
			local filter = inFilter
			if filter ~= '' then filter = filter .. '\\' end
			filter = filter .. entry.folder
			self:_WriteFiles(entry, filter)
		else
			table.insert(self.Contents, '    <None Include="' .. entry:gsub('/', '\\') .. '">\n')
			table.insert(self.Contents, '      <Filter>' .. inFilter .. '</Filter>\n')
			table.insert(self.Contents, '    </None>\n')
		end
	end
end


function VisualStudio201xProjectMetaTable:_WriteFilesFlat(folder)
	for entry in ivalues(folder) do
		if type(entry) == 'table' then
			self:_WriteFilesFlat(entry)
		else
			table.insert(self.Contents, '    <None Include="' .. entry:gsub('/', '\\') .. '" />\n')
		end
	end
end






local VisualStudio201xSolutionMetaTable = {  __index = VisualStudio201xSolutionMetaTable  }

function VisualStudio201xSolutionMetaTable:_GatherSolutionFolders(folder, folderList, fullPath)
	for entry in ivalues(folder) do
		if type(entry) == 'table' then
			local solutionFolder = fullPath .. '\\' .. entry.folder
			table.insert(folderList, solutionFolder)
			self:_GatherSolutionFolders(entry, folderList, solutionFolder)
		end
	end
end


function VisualStudio201xSolutionMetaTable:_WriteNestedProjects(folder, fullPath)
	for entry in ivalues(folder) do
		if type(entry) == 'table' then
			local solutionFolder = fullPath .. '\\' .. entry.folder
			if folder.folder then
				table.insert(self.Contents, expand([[
		$(Child) = $(Parent)
]], {  Child = ProjectExportInfo[solutionFolder].Uuid, Parent = ProjectExportInfo[fullPath].Uuid  }))
			end
			self:_WriteNestedProjects(entry, solutionFolder)
		else
			if folder.folder then
				table.insert(self.Contents, expand([[
		$(Child) = $(Parent)
]], {  Child = ProjectExportInfo[entry].Uuid, Parent = ProjectExportInfo[fullPath].Uuid  }))
			end
		end
	end
end


function VisualStudio201xSolutionMetaTable:Write(outputPath)
	local filename = outputPath .. self.Name .. '.sln'

	local workspace = Workspaces[self.Name]

	-- Write header.
	table.insert(self.Contents, '\xef\xbb\xbf\n')

	table.insert(self.Contents, [[
Microsoft Visual Studio Solution File, Format Version 11.00
# Visual Studio 2010
]])

	-- Write projects.
	for projectName in ivalues(workspace.Projects) do
		local info = ProjectExportInfo[projectName]
		if info then
			table.insert(self.Contents, expand([[
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "$(Name)", "$(Filename)", "$(Uuid)"
EndProject
]], info))
		end
	end

	-- Write the folders we use.
	local folderList = {}
	self:_GatherSolutionFolders(workspace.ProjectTree, folderList, '')

	for solutionFolderName in ivalues(folderList) do
		local info = ProjectExportInfo[solutionFolderName]
		if not info then
			info =
			{
				Name = solutionFolderName:match('.*\\(.+)'),
				Filename = solutionFolderName,
				Uuid = '{' .. uuid.new():upper() .. '}'
			}
			ProjectExportInfo[solutionFolderName] = info
		end

		table.insert(self.Contents, expand([[
Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "$(Name)", "$(Name)", "$(Uuid)"
EndProject
]], info))
	end

	-- Begin writing the Global section.
	table.insert(self.Contents, [[
Global
]])

	table.insert(self.Contents, [[
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
]])

	for platformName in ivalues(Config.Platforms) do
		for configName in ivalues(Config.Configurations) do
			local configInfo =
			{
				VSPlatform = GetMapPlatformToVSPlatform(platformName),
				VSConfig = GetMapConfigToVSConfig(configName),
			}
			table.insert(self.Contents, expand([[
		$(VSConfig)|$(VSPlatform) = $(VSConfig)|$(VSPlatform)
]], configInfo))
		end
	end

	table.insert(self.Contents, [[
	EndGlobalSection
]])

	-------------------
	table.insert(self.Contents, [[
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
]])

	for platformName in ivalues(Config.Platforms) do
		for configName in ivalues(Config.Configurations) do
			local info = ProjectExportInfo[buildWorkspaceName]
			local configInfo =
			{
				VSPlatform = GetMapPlatformToVSPlatform(platformName),
				VSConfig = GetMapConfigToVSConfig(configName),
				RealVSPlatform = RealVSPlatform(platformName),
				RealVSConfig = RealVSConfig(platformName, configName),
			}
			table.insert(self.Contents, expand([[
		$(Uuid).$(VSConfig)|$(VSPlatform).ActiveCfg = $(RealVSConfig)|$(RealVSPlatform)
]], configInfo, info))

			table.insert(self.Contents, expand([[
		$(Uuid).$(VSConfig)|$(VSPlatform).Build.0 = $(RealVSConfig)|$(RealVSPlatform)
]], configInfo, info))
		end
	end

	for platformName in ivalues(Config.Platforms) do
		for configName in ivalues(Config.Configurations) do
			local info = ProjectExportInfo[updateWorkspaceName]
			local configInfo =
			{
				VSPlatform = GetMapPlatformToVSPlatform(platformName),
				VSConfig = GetMapConfigToVSConfig(configName),
				RealVSPlatform = RealVSPlatform(platformName),
				RealVSConfig = RealVSConfig(platformName, configName),
			}
			table.insert(self.Contents, expand([[
		$(Uuid).$(VSConfig)|$(VSPlatform).ActiveCfg = $(RealVSConfig)|$(RealVSPlatform)
]], configInfo, info))
		end
	end

	for projectName in ivalues(workspace.Projects) do
		local info = ProjectExportInfo[projectName]
		if info then
			for platformName in ivalues(Config.Platforms) do
				for configName in ivalues(Config.Configurations) do
					local configInfo =
					{
						VSPlatform = GetMapPlatformToVSPlatform(platformName),
						VSConfig = GetMapConfigToVSConfig(configName),
						RealVSPlatform = RealVSPlatform(platformName),
						RealVSConfig = RealVSConfig(platformName, configName),
					}
					table.insert(self.Contents, expand([[
		$(Uuid).$(VSConfig)|$(VSPlatform).ActiveCfg = $(RealVSConfig)|$(RealVSPlatform)
]], configInfo, info))
				end
			end
		end
	end

	table.insert(self.Contents, [[
	EndGlobalSection
]])

	table.insert(self.Contents, [[
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
]])

	table.insert(self.Contents, [[
	GlobalSection(NestedProjects) = preSolution
]])

	self:_WriteNestedProjects(workspace.ProjectTree, '')

	table.insert(self.Contents, [[
	EndGlobalSection
]])

	-- Write EndGlobal section.
	table.insert(self.Contents, [[
EndGlobal
]])

	self.Contents = table.concat(self.Contents):gsub('\r\n', '\n'):gsub('\n', '\r\n')

	WriteFileIfModified(filename, self.Contents)
end

function VisualStudio201xSolution(solutionName, options)
	return setmetatable(
		{
			Contents = {},
			Name = solutionName,
			Options = options,
		}, { __index = VisualStudio201xSolutionMetaTable }
	)
end



function VisualStudio201xInitialize()
	local outPath = os.path.combine(destinationRootPath, '_workspace.' .. opts.gen .. '_') .. '/'
	local chunk = loadfile(outPath .. 'VSProjectExportInfo.lua')
	if chunk then chunk() end
	if not ProjectExportInfo then
		ProjectExportInfo = {}
	end
end


function VisualStudio201xShutdown()
	local outPath = os.path.combine(destinationRootPath, '_workspace.' .. opts.gen .. '_') .. '/'
	LuaDumpObject(outPath .. 'VSProjectExportInfo.lua', 'ProjectExportInfo', ProjectExportInfo)
end




