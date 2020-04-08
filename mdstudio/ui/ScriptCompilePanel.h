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
#include <functional>

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

	bool BeginCompileAsync(const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::string>& defines, std::function<void(const std::vector<std::string>& symbolOutput)> const& onFinished);
	bool CompileBlocking(const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::string>& defines);

	void AppendText(const wxString& text);

	void OnProcessFinished(int pid, int status);

	const std::vector<std::string>& GetSymbolOutput() const { return m_symbolOutput; }

private:
	enum State
	{
		Idle,
		Compiling,
		Copying,
		ReadingSymbols
	};

	bool BeginObjCopy(const std::string& filename, const std::string& outname);
	bool BeginSymbolRead(const std::string& filename, const std::string& outname);

	void CollectOutput();

	Project& m_project;
	MainWindow* m_mainWindow;

#if defined BEEHIVE_PLUGIN_LUMINARY
	luminary::ScriptCompiler m_scriptCompiler;
#endif

	std::string m_currentFilename;
	std::string m_currentOutname;
	std::vector<std::string> m_symbolOutput;
	std::function<void(const std::vector<std::string>& symbolOutput)> m_onFinished;
	wxWeakRef<ScriptCompilerRunner> m_compileRunner;
	State m_state;
};

class ScriptCompilerRunner : public wxProcess
{
public:
	ScriptCompilerRunner(ScriptCompilePanel& panel) : m_panel(panel) {}
	virtual void OnTerminate(int pid, int status);

private:
	ScriptCompilePanel& m_panel;
};