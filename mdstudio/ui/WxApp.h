///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <wx/setup.h>
#include <wx/wx.h>

class MainWindow;

class MDStudioWxApp : public wxApp
{
public:
	MDStudioWxApp();
	virtual ~MDStudioWxApp();

	virtual bool OnInit();

	MDStudioWxApp& wxGetApp() { return *static_cast<MDStudioWxApp*>(wxApp::GetInstance()); }

	wxWeakRef<MainWindow> m_mainWindow;
};