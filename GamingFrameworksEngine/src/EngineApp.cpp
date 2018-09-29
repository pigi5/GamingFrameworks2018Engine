#include "../header/EngineApp.h"

EngineApp::EngineApp()
{
}

EngineApp::~EngineApp()
{
}

bool EngineApp::OnInit()
{
	Gui* mainFrame = new Gui("Frameworks Engine");
	mainFrame->Show(true);
	return true;
}

wxIMPLEMENT_APP(EngineApp);