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

#include "AssemblerPanel.h"
#include "MainWindow.h"
#include "Dialogs.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/txtstrm.h>

#include <ion/core/thread/Sleep.h>

AssemblerPanel::AssemblerPanel(MainWindow* mainWindow, Project& project, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ScriptCompilePanelBase(parent, id, pos, size, style)
	, m_project(project)
	, m_mainWindow(mainWindow)
{
	m_state = State::Idle;
}

bool AssemblerPanel::BeginAssembleAsync(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines, std::function<void()> const& onFinished)
{
	m_state = State::Assembling;

	m_textOutput->Clear();
	m_onFinished = onFinished;
	m_compileRunner = new AssemblerRunner(*this);
	m_compileRunner->Redirect();
	m_currentFilename = filename;
	m_currentOutname = outname;

	std::string commandLine = GenerateCommandLine(assembler, filename, outname, includes, defines);
	wxExecuteEnv env;
	env.cwd = ion::string::GetDirectory(filename);

	m_textOutput->AppendText(commandLine);

	if (wxExecute(commandLine, wxEXEC_ASYNC, m_compileRunner, &env) < 0)
	{
		m_textOutput->AppendText("Error starting assembler");
		return false;
	}

	return true;
}

bool AssemblerPanel::AssembleBlocking(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines)
{
	m_textOutput->Clear();
	m_onFinished = nullptr;
	m_compileRunner = new AssemblerRunner(*this);
	m_compileRunner->Redirect();
	m_currentFilename = filename;
	m_state = State::Assembling;

	std::string commandLine = GenerateCommandLine(assembler, filename, outname, includes, defines);
	wxExecuteEnv env;
	env.cwd = ion::string::GetDirectory(filename);

	m_textOutput->AppendText(commandLine);

	long compileResult = wxExecute(commandLine, wxEXEC_SYNC, m_compileRunner, &env);
	if (compileResult < 0)
	{
		m_textOutput->AppendText("Error starting assembler");
		return false;
	}
	else if (compileResult > 0)
	{
		CollectOutput();
		m_textOutput->AppendText("Assembly error");
		return false;
	}

	CollectOutput();
	m_state = State::Idle;

	return true;
}

std::string AssemblerPanel::GenerateCommandLine(const std::string& assembler, const std::string& filename, const std::string& outname, const std::vector<std::string>& includes, const std::vector<std::pair<std::string, std::string>>& defines)
{
	std::string cmdline;
	std::string assemblyArgs;
	std::string includeArgs;
	std::string defineArgs;
	std::string flags = "/p /c /zd /w /k ";

	if (includes.size())
	{
		for (auto include : includes)
		{
			includeArgs += "/j " + include + "\\* ";
		}

		includeArgs += " ";
	}

	if (defines.size())
	{
		defineArgs += "/e ";

		for(int i = 0; i < defines.size(); i++)
		{
			defineArgs += defines[i].first + "=" + defines[i].second;
			if (i < (defines.size() - 1))
				defineArgs += ";";
		}

		defineArgs += " ";
	}

	cmdline += assembler + " " + flags + includeArgs + defineArgs + filename + "," + outname;

	return cmdline;
}

void AssemblerPanel::CollectOutput()
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

void AssemblerPanel::AppendText(const wxString& text)
{
	m_textOutput->AppendText(text);
}

void AssemblerPanel::OnProcessFinished(int pid, int status)
{
	if (m_state == State::Assembling)
	{
		AppendText("Assembler with process ID " + std::to_string(pid) + " exited with status " + std::to_string(status) + "\n");
		m_state = State::Idle;

		if (m_onFinished)
		{
			m_onFinished();
		}
	}
}

void AssemblerRunner::OnTerminate(int pid, int status)
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