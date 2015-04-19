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
	m_resourceManager = NULL;
	m_project = NULL;
}

MDStudio::~MDStudio()
{
}

bool MDStudio::Initialise()
{
	//Create resource manager
	m_resourceManager = new ion::io::ResourceManager();

	//Set resource directories
	m_resourceManager->SetResourceDirectory<ion::render::Shader>("shaders");
	m_resourceManager->SetResourceDirectory<ion::render::Texture>("textures");
	m_resourceManager->SetResourceDirectory<ion::render::Material>("materials");

	//Create new project by default
	m_project = new Project();

	m_wxApp = new MDStudioWxApp(*m_resourceManager);

	wxApp::SetInstance(m_wxApp);
	wxEntryStart(0, NULL);
	wxTheApp->OnInit();
	m_wxApp->GetMainWindow()->Maximize(true);
	m_wxApp->GetMainWindow()->SetProject(m_project);
	wxTheApp->OnRun();

	return false;
}

void MDStudio::Shutdown()
{
	wxTheApp->OnExit();
	wxEntryCleanup();

	delete m_resourceManager;
	delete m_project;
}

bool MDStudio::Update(float deltaTime)
{
	return false;
}

void MDStudio::Render()
{

}
