///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "WxApp.h"
#include "MainWindow.h"

wxAppConsole *wxCreateApp()
{
	ion::debug::Error("Not supported");
	return NULL;
	//wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "MDStudio");
	//return new MDStudioWxApp;
}

MDStudioWxApp::MDStudioWxApp(ion::io::ResourceManager& resourceManager)
	: m_resourceManager(resourceManager)
{
}

MDStudioWxApp::~MDStudioWxApp()
{
}

bool MDStudioWxApp::OnInit()
{
	m_mainWindow = new MainWindow(m_resourceManager);
	SetTopWindow(m_mainWindow);
    m_mainWindow->Show(true);

	return true;
}