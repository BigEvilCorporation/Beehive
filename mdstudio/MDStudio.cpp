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

	//TESTING
	int testTiles[4][8*8] = 
	{
		{
			1,1,1,1,0,0,0,0,
			1,1,1,1,0,0,0,0,
			1,1,1,1,0,0,0,0,
			1,1,1,1,0,0,0,0,
			0,0,0,0,2,2,2,2,
			0,0,0,0,2,2,2,2,
			0,0,0,0,2,2,2,2,
			0,0,0,0,2,2,2,2
		},

		{
			3,3,3,3,3,3,3,3,
			3,3,3,3,3,3,3,0,
			3,3,3,3,3,3,0,0,
			3,3,3,3,3,0,0,0,
			3,3,3,3,0,0,0,0,
			3,3,3,0,0,0,0,0,
			3,3,0,0,0,0,0,0,
			3,0,0,0,0,0,0,0
		},

		{
			4,4,4,4,4,4,4,4,
			4,0,0,0,0,0,0,4,
			4,0,4,4,4,4,0,4,
			4,0,4,0,0,4,0,4,
			4,0,4,0,0,4,0,4,
			4,0,4,4,4,4,0,4,
			4,0,0,0,0,0,0,4,
			4,4,4,4,4,4,4,4
		},

		{
			5,5,5,5,5,5,5,5,
			5,0,0,0,0,0,0,5,
			5,0,0,0,0,0,0,5,
			5,0,0,0,0,0,0,5,
			5,0,0,0,0,0,0,5,
			5,0,0,0,0,0,0,5,
			5,0,0,0,0,0,0,5,
			5,5,5,5,5,5,5,5
		}
	};

	for(int j = 0; j < 4; j++)
	{
		TileId tileId = m_project->GetMap().GetTileset().AddTile();
		Tile* tile = m_project->GetMap().GetTileset().GetTile(tileId);

		for(int i = 0; i < 8*8; i++)
		{
			int y = i / 8;
			int x = i % 8;
			tile->SetPixelColour(x, y, testTiles[j][i]);
		}
	}

	Palette* palette = m_project->GetPalette(0);
	palette->SetColour(0, Colour(1.0f, 1.0f, 1.0f));
	palette->SetColour(1, Colour(1.0f, 0.0f, 0.0f));
	palette->SetColour(2, Colour(0.0f, 1.0f, 0.0f));
	palette->SetColour(3, Colour(0.0f, 0.0f, 1.0f));
	palette->SetColour(4, Colour(1.0f, 1.0f, 0.0f));
	palette->SetColour(5, Colour(0.0f, 1.0f, 1.0f));

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
