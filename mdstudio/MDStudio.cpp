///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "MDStudio.h"

#include "ui/WxApp.h"
#include "ui/MainWindow.h"

MDStudio::MDStudio()
	: ion::framework::Application("MDStudio")
{
	m_project = NULL;
}

MDStudio::~MDStudio()
{
}

bool MDStudio::Initialise()
{
	//Create new project by default
	m_project = new Project();

	m_wxApp = new MDStudioWxApp();

	wxApp::SetInstance(m_wxApp);
	wxEntryStart(0, NULL);
	wxTheApp->OnInit();
	m_wxApp->m_mainWindow->Maximize(true);
	m_wxApp->m_mainWindow->SetProject(m_project);
	wxTheApp->OnRun();

	return false;
}

void MDStudio::Shutdown()
{
	wxTheApp->OnExit();
	wxEntryCleanup();
}

bool MDStudio::Update(float deltaTime)
{
	return false;
}

void MDStudio::Render()
{

}
