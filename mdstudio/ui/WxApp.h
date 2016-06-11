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