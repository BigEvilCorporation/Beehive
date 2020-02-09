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

#include "ScriptCompilePanel.h"
#include "MainWindow.h"
#include "Dialogs.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/txtstrm.h>

const std::string g_compilerDir = "compiler\\m68k-elf\\bin";
const std::string g_includeDir = "scripts\\common";

ScriptCompilePanel::ScriptCompilePanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ScriptCompilePanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{

}

void ScriptCompilePanel::BeginCompile(const std::string& filename)
{
	m_textOutput->Clear();
	m_compileRunner = new ScriptCompilerRunner(*this);
	m_compileRunner->Redirect();

#if defined BEEHIVE_PLUGIN_LUMINARY
	m_currentFilename = filename;
	std::string commandLine = m_scriptCompiler.GenerateCompileCommand(filename, g_compilerDir, g_includeDir);
	if (wxExecute(commandLine, wxEXEC_ASYNC, m_compileRunner) < 0)
	{
		m_textOutput->AppendText("Error starting script compiler");
	}
#endif
}

void ScriptCompilePanel::BeginObjCopy(const std::string& filename)
{

}

void ScriptCompilePanel::AppendText(const wxString& text)
{
	m_textOutput->AppendText(text);
}

void ScriptCompilerRunner::OnTerminate(int pid, int status)
{
	if (IsInputAvailable())
	{
		wxTextInputStream stream(*GetInputStream());
		while (GetInputStream()->IsOk() && !GetInputStream()->Eof())
		{
			m_panel.AppendText(stream.ReadLine() + "\n");
		}
	}
	else if (IsErrorAvailable())
	{
		wxTextInputStream stream(*GetErrorStream());
		while (GetErrorStream()->IsOk() && !GetErrorStream()->Eof())
		{
			m_panel.AppendText(stream.ReadLine() + "\n");
		}
	}
	
	m_panel.AppendText("Script compiler with process ID " + std::to_string(pid) + " exited with status " + std::to_string(status));
}