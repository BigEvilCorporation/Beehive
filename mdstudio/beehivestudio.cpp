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

#include "BeehiveStudio.h"

#include "ui/WxApp.h"
#include "ui/MainWindow.h"

Beehive::Beehive()
	: ion::framework::Application("Beehive")
{
	m_project = NULL;
}

Beehive::~Beehive()
{
}

bool Beehive::Initialise()
{
	m_wxApp = new BeehiveWxApp();

	wxApp::SetInstance(m_wxApp);
	wxEntryStart(0, NULL);
	wxTheApp->OnInit();
	m_wxApp->GetMainWindow()->Maximize(true);
	wxTheApp->OnRun();

	return false;
}

void Beehive::Shutdown()
{
	wxTheApp->OnExit();
	wxEntryCleanup();
}

bool Beehive::Update(float deltaTime)
{
	return false;
}

void Beehive::Render()
{

}
