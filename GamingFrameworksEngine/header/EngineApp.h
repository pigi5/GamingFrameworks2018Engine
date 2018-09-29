#pragma once
#include <wx/wxprec.h>
#include "../header/Gui.h"

class EngineApp : public wxApp
{
public:
	EngineApp();
	virtual ~EngineApp();
	virtual bool OnInit() override;
};