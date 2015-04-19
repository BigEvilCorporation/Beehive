///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <wx/setup.h>
#include <wx/wx.h>

#include <ion/io/ResourceManager.h>

class MainWindow;

class MDStudioWxApp : public wxApp
{
public:
	MDStudioWxApp(ion::io::ResourceManager& resourceManager);
	virtual ~MDStudioWxApp();

	virtual bool OnInit();

	MDStudioWxApp& wxGetApp() { return *static_cast<MDStudioWxApp*>(wxApp::GetInstance()); }
	wxWeakRef<MainWindow> GetMainWindow() const { return m_mainWindow; }

private:
	ion::io::ResourceManager& m_resourceManager;
	wxWeakRef<MainWindow> m_mainWindow;
};