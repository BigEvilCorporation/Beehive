///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <wx/setup.h>
#include <wx/wx.h>

class MainWindow;

class BeehiveWxApp : public wxApp
{
public:
	BeehiveWxApp();
	virtual ~BeehiveWxApp();

	virtual bool OnInit();

	BeehiveWxApp& wxGetApp() { return *static_cast<BeehiveWxApp*>(wxApp::GetInstance()); }
	wxWeakRef<MainWindow> GetMainWindow() const { return m_mainWindow; }

private:
	wxWeakRef<MainWindow> m_mainWindow;
};