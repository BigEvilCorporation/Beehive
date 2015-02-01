#include "MDStudio.h"

#include "ui/WxApp.h"

MDStudio::MDStudio()
	: ion::framework::Application("MDStudio")
{

}

MDStudio::~MDStudio()
{
}

bool MDStudio::Initialise()
{
	wxApp::SetInstance(new MDStudioWxApp());
	wxEntryStart(0, NULL);
	wxTheApp->OnInit();
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
	bool exit = false;


	return !exit;
}

void MDStudio::Render()
{

}
