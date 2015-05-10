///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "WxApp.h"
#include "MainWindow.h"

wxAppConsole *wxCreateApp()
{
	ion::debug::Error("Not supported");
	return NULL;
}

BeehiveWxApp::BeehiveWxApp()
{
}

BeehiveWxApp::~BeehiveWxApp()
{
}

bool BeehiveWxApp::OnInit()
{
	m_mainWindow = new MainWindow();
	SetTopWindow(m_mainWindow);
    m_mainWindow->Show(true);

	return true;
}