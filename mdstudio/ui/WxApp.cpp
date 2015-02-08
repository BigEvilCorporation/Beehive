///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

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
	SetTopWindow(m_mainWindow);
    m_mainWindow->Show(true);

	return true;
}