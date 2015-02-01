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
	//wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp);
	MDStudioWxApp wxApp;

	int numArgs = 0;
	wxApp.Initialize(numArgs, NULL);
	return true;
}

void MDStudio::Shutdown()
{

}

bool MDStudio::Update(float deltaTime)
{
	bool exit = false;


	return !exit;
}

void MDStudio::Render()
{

}
