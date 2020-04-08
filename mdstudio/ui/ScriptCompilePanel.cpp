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

#include <ion/core/thread/Sleep.h>

const std::string g_compilerDir = "compiler\\m68k-elf";
const std::string g_compilerVer = "4.8.0";

ScriptCompilePanel::ScriptCompilePanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ScriptCompilePanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_state = State::Idle;
}

bool ScriptCompilePanel::BeginCompileAsync(const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::string>& defines, std::function<void(const std::vector<std::string>& symbolOutput)> const& onFinished)
{
	m_state = State::Compiling;

	m_textOutput->Clear();
	m_onFinished = onFinished;
	m_compileRunner = new ScriptCompilerRunner(*this);
	m_compileRunner->Redirect();

#if defined BEEHIVE_PLUGIN_LUMINARY
	m_currentFilename = filename;
	m_currentOutname = outname;
	std::string commandLine = m_scriptCompiler.GenerateCompileCommand(filename, outname, g_compilerDir, includes, defines);
	wxExecuteEnv env;
	env.cwd = ion::io::FileDevice::GetDefault()->GetMountPoint() + "\\" + ion::io::FileDevice::GetDefault()->GetDirectory();
	env.env["PATH"] = m_scriptCompiler.GetBinPath(g_compilerDir) + ";" + m_scriptCompiler.GetLibExecPath(g_compilerDir, g_compilerVer);
	if (wxExecute(commandLine, wxEXEC_ASYNC, m_compileRunner, &env) < 0)
	{
		m_textOutput->AppendText("Error starting script compiler");
		return false;
	}
#endif

	return true;
}

bool ScriptCompilePanel::CompileBlocking(const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::string>& defines)
{
	m_textOutput->Clear();
	m_symbolOutput.clear();
	m_onFinished = nullptr;
	m_compileRunner = new ScriptCompilerRunner(*this);
	m_compileRunner->Redirect();

#if defined BEEHIVE_PLUGIN_LUMINARY
	m_currentFilename = filename;

	m_state = State::Compiling;
	std::string compileCmd = m_scriptCompiler.GenerateCompileCommand(filename, outname, g_compilerDir, includes, defines);
	wxExecuteEnv env;
	env.env["PATH"] = m_scriptCompiler.GetBinPath(g_compilerDir) + ";" + m_scriptCompiler.GetLibExecPath(g_compilerDir, g_compilerVer);
	if (wxExecute(compileCmd, wxEXEC_SYNC, m_compileRunner, &env) < 0)
	{
		m_textOutput->AppendText("Error starting script compiler");
		return false;
	}

	CollectOutput();

	m_state = State::Copying;
	std::string objcopyCmd = m_scriptCompiler.GenerateObjCopyCommand(filename, outname, g_compilerDir);
	if (wxExecute(objcopyCmd, wxEXEC_SYNC, m_compileRunner) < 0)
	{
		m_textOutput->AppendText("Error starting objcopy");
		return false;
	}

	CollectOutput();

	m_state = State::ReadingSymbols;
	std::string symbolsCmd = m_scriptCompiler.GenerateSymbolReadCommand(filename, outname, g_compilerDir);
	if (wxExecute(symbolsCmd, wxEXEC_SYNC, m_compileRunner) < 0)
	{
		m_textOutput->AppendText("Error starting symbol reader");
		return false;
	}

	CollectOutput();
	m_state = State::Idle;
#endif

	return true;
}

bool ScriptCompilePanel::BeginObjCopy(const std::string& filename, const std::string& outname)
{
	m_state = State::Copying;

	m_compileRunner = new ScriptCompilerRunner(*this);
	m_compileRunner->Redirect();

#if defined BEEHIVE_PLUGIN_LUMINARY
	m_currentFilename = filename;
	std::string commandLine = m_scriptCompiler.GenerateObjCopyCommand(filename, outname, g_compilerDir);
	if (wxExecute(commandLine, wxEXEC_ASYNC, m_compileRunner) < 0)
	{
		m_textOutput->AppendText("Error starting objcopy");
		return false;
	}
#endif

	return true;
}

bool ScriptCompilePanel::BeginSymbolRead(const std::string& filename, const std::string& outname)
{
	m_state = State::ReadingSymbols;
	m_symbolOutput.clear();

	m_compileRunner = new ScriptCompilerRunner(*this);
	m_compileRunner->Redirect();

#if defined BEEHIVE_PLUGIN_LUMINARY
	m_currentFilename = filename;
	std::string commandLine = m_scriptCompiler.GenerateSymbolReadCommand(filename, outname, g_compilerDir);
	if (wxExecute(commandLine, wxEXEC_ASYNC, m_compileRunner) < 0)
	{
		m_textOutput->AppendText("Error starting symbol reader");
		return false;
	}
#endif

	return true;
}

void ScriptCompilePanel::CollectOutput()
{
	if (m_compileRunner->IsInputAvailable())
	{
		wxTextInputStream stream(*m_compileRunner->GetInputStream());
		while (m_compileRunner->GetInputStream()->IsOk() && !m_compileRunner->GetInputStream()->Eof())
		{
			AppendText(stream.ReadLine() + "\n");
		}
	}
	else if (m_compileRunner->IsErrorAvailable())
	{
		wxTextInputStream stream(*m_compileRunner->GetErrorStream());
		while (m_compileRunner->GetErrorStream()->IsOk() && !m_compileRunner->GetErrorStream()->Eof())
		{
			AppendText(stream.ReadLine() + "\n");
		}
	}
}

void ScriptCompilePanel::AppendText(const wxString& text)
{
	m_textOutput->AppendText(text);

	if (m_state == State::ReadingSymbols)
	{
		m_symbolOutput.push_back(text.c_str().AsChar());
	}
}

void ScriptCompilePanel::OnProcessFinished(int pid, int status)
{
	if (m_state == State::Compiling)
	{
		AppendText("Script compiler with process ID " + std::to_string(pid) + " exited with status " + std::to_string(status) + "\n");
		if (!BeginObjCopy(m_currentFilename, m_currentOutname))
		{
			m_state = State::Idle;
		}
	}
	else if(m_state == State::Copying)
	{
		AppendText("Object copier with process ID " + std::to_string(pid) + " exited with status " + std::to_string(status) + "\n");
		if(!BeginSymbolRead(m_currentFilename, m_currentOutname))
		{
			m_state = State::Idle;
		}
	}
	else if (m_state == State::ReadingSymbols)
	{
		AppendText("Symbol reader with process ID " + std::to_string(pid) + " exited with status " + std::to_string(status) + "\n");
		m_state = State::Idle;

		if (m_onFinished)
		{
			m_onFinished(m_symbolOutput);
		}
	}
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
	
	m_panel.OnProcessFinished(pid, status);
}