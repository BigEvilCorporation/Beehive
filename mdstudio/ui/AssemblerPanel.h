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

class MainWindow;
class AssemblerRunner;

class AssemblerPanel : public ScriptCompilePanelBase
{
public:
	AssemblerPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr);

	bool BeginAssembleAsync(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines, std::function<void()> const& onFinished);
	bool AssembleBlocking(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines);

	void AppendText(const wxString& text);

	void OnProcessFinished(int pid, int status);

private:
	enum State
	{
		Idle,
		Assembling,
	};

	std::string GenerateCommandLine(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines);
	void CollectOutput();

	Project& m_project;
	MainWindow* m_mainWindow;

	std::string m_currentFilename;
	std::string m_currentOutname;
	std::function<void()> m_onFinished;
	wxWeakRef<AssemblerRunner> m_compileRunner;
	State m_state;
};

class AssemblerRunner : public wxProcess
{
public:
	AssemblerRunner(AssemblerPanel& panel) : m_panel(panel) {}
	virtual void OnTerminate(int pid, int status);

private:
	AssemblerPanel& m_panel;
};