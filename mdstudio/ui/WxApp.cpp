#include "WxApp.h"
#include "MainWindow.h"

wxAppConsole *wxCreateApp()
{
	wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "MDStudio");
	return new MDStudioWxApp;
}

MDStudioWxApp::MDStudioWxApp()
{
}

MDStudioWxApp::~MDStudioWxApp()
{
}

bool MDStudioWxApp::OnInit()
{
	MainWindow* frame = new MainWindow("MD Studio", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);

	return true;
}