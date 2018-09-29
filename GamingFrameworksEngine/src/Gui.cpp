#include "../header/Gui.h"

Gui::Gui(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	Center();
}