#include "WxApp.h"
#include "MainWindow.h"

wxAppConsole *wxCreateApp()
{
	wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "MDStudio");
	return new MDStudioWxApp;
}

MDStudioWxApp::MDStudioWxApp()
{
}

MDStudioWxApp::~MDStudioWxApp()
{
}

bool MDStudioWxApp::OnInit()
{
	m_mainWindow = new MainWindow();
    m_mainWindow->Show(true);

	return true;
}