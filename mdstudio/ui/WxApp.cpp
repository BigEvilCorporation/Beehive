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
	m_mainWindow->OnPostInit();

	return true;
}