///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include <ion/beehive/Project.h>

#include <map>
#include <vector>

#include <wx/process.h>

#include "UIBase.h"

#if defined BEEHIVE_PLUGIN_LUMINARY
#include <luminary/Types.h>
#include <luminary/ScriptCompiler.h>
#endif

class MainWindow;
class ScriptCompilerRunner;

class ScriptCompilePanel : public ScriptCompilePanelBase
{
public:
	ScriptCompilePanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	void BeginCompile(const std::string& filename);
	void BeginObjCopy(const std::string& filename);

	void AppendText(const wxString& text);

private:
	Project& m_project;
	MainWindow* m_mainWindow;

#if defined BEEHIVE_PLUGIN_LUMINARY
	luminary::ScriptCompiler m_scriptCompiler;
#endif

	std::string m_currentFilename;
	wxWeakRef<ScriptCompilerRunner> m_compileRunner;
};

class ScriptCompilerRunner : public wxProcess
{
public:
	ScriptCompilerRunner(ScriptCompilePanel& panel) : m_panel(panel) {}
	virtual void OnTerminate(int pid, int status);

private:
	ScriptCompilePanel& m_panel;
};