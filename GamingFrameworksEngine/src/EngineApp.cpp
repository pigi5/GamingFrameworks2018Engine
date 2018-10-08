#include "wx/wxprec.h"
#include "wx/splitter.h"
#include "wx/dcmirror.h"
#include "wx/dir.h"
#include "wx/file.h"
#include "../header/Engine.h"
#include "../header/Utils.h"
#include <direct.h>
#include <iostream>

string operation;
wxListBox* listbox;
wxString currentPath;
string selObject;
string selTrigger;


// ID for the menu commands
enum
{
	QUIT = 1,
	NEW = 2,
	OPEN = 3,
	SAVE = 4,
	ROOM = 5,
	OBJECT = 6,
	SPRITE = 7,
	AUDIO = 8,
	MATERIAL = 9,
	OVERLAY = 10,
	NEW_ITEM = 11,
	EDIT_ITEM = 12,
	DELETE_ITEM = 13,
	CLEAR = 14,
	PLAY = 15
};

// ----------------------------------------------------------------------------
// our classes
// ----------------------------------------------------------------------------

class MyApp : public wxApp
{
public:
	MyApp() { }

	virtual bool OnInit() wxOVERRIDE;
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
	virtual ~MyFrame();

	// Menu commands

	void OnQuit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnPlay(wxCommandEvent& event);
    
    void loadConfig();
    void unloadConfig();
    void saveConfig();
    void reloadConfig();

	wxWindow *m_left, *m_right;

	wxSplitterWindow* m_splitter;
	wxWindow *m_replacewindow;

	wxDECLARE_EVENT_TABLE();
};

class MySplitterWindow : public wxSplitterWindow
{
public:
	MySplitterWindow(wxFrame *parent);
	void OnChange();
	wxFrame *m_frame;
};

class Sidebar : public wxScrolledWindow
{
public:
	Sidebar(wxWindow* parent);
	wxWindow* parent;
	virtual ~Sidebar() {};

	void onSprite(wxCommandEvent& event);
	void onAudio(wxCommandEvent& event);
	void onObject(wxCommandEvent& event);
	void onRoom(wxCommandEvent& event);
	void onMaterial(wxCommandEvent& event);
	void onOverlay(wxCommandEvent& event);
	void onNew(wxCommandEvent& event);
	void onSelect(wxCommandEvent& event);
	void onDelete(wxCommandEvent& event);

private:
};

class Editor : public wxScrolledWindow
{
public:
	Editor(wxWindow* parent);
	virtual ~Editor() {};

	void resetTrigger();
	void resetAction();
	void resetCon();
	
	void onNew(wxCommandEvent& event);
	void onEdit(wxCommandEvent& event);
	void onDelete(wxCommandEvent& event);

	void onBox1Select(wxCommandEvent& event);

	wxListBox* lb1;
	wxListBox* lb2;
	wxListBox* lb3;
	wxListBox* lb4;
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	// create and show the main frame
	MyFrame* frame = new MyFrame;

	frame->Show(true);

	return true;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)

EVT_MENU(QUIT, MyFrame::OnQuit)
EVT_MENU(NEW, MyFrame::OnNew)
EVT_MENU(OPEN, MyFrame::OnOpen)
EVT_MENU(SAVE, MyFrame::OnSave)
EVT_MENU(PLAY, MyFrame::OnPlay)

wxEND_EVENT_TABLE()

// My frame constructor
MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, wxT("Frameworks Engine"),
		wxDefaultPosition, wxSize(800, 600),
		wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{

	// Make a menubar
	wxMenu *menu = new wxMenu;
	wxMenu *game = new wxMenu;

	menu->Append(NEW, wxT("&New"));
	menu->Append(OPEN, wxT("&Open"));
	menu->Append(SAVE, wxT("&Save"));
	menu->Append(QUIT, wxT("&Quit"));

	game->Append(PLAY, wxT("&Play"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menu, wxT("&File"));
	menuBar->Append(game, wxT("&Game"));

	SetMenuBar(menuBar);

	m_splitter = new MySplitterWindow(this);
	m_splitter->SetSize(GetClientSize());
	m_splitter->SetSashGravity(0.0);
	m_splitter->SetMinimumPaneSize(50);

	m_left = new Sidebar(m_splitter);
	m_left->SetBackgroundColour(*wxLIGHT_GREY);

	m_right = new Editor(m_splitter);
	m_right->SetBackgroundColour(*wxLIGHT_GREY);

	m_splitter->SplitVertically(m_left, m_right, this->GetSize().GetX()/5);

	m_replacewindow = NULL;
}

MyFrame::~MyFrame()
{
	if (m_replacewindow) {
		m_replacewindow->Destroy();
	}
}

void MyFrame::loadConfig()
{
    try 
    {
	    loadAll<Material>(currentPath.ToStdString());
	    loadAll<Sprite>(currentPath.ToStdString());
	    // load shallow first so we can have all the name references
	    loadAll<ActorType>(currentPath.ToStdString(), true);
	    loadAll<ActorType>(currentPath.ToStdString());
	    // load shallow first so we can have all the name references
	    loadAll<OverlayType>(currentPath.ToStdString(), true);
	    loadAll<OverlayType>(currentPath.ToStdString());
	    loadAll<Room>(currentPath.ToStdString());
    }
	catch (const ConfigurationError& e)
	{
		std::cerr << e.what() << std::endl;
		wxMessageDialog* err = new wxMessageDialog(this, e.what(), "ERROR", wxICON_ERROR | wxOK | wxCENTRE);
		err->ShowModal();
	}
    catch (const std::exception &e)
    {
		std::cerr << e.what() << std::endl;
		wxMessageDialog* err = new wxMessageDialog(this, e.what(), "ERROR", wxICON_ERROR | wxOK | wxCENTRE);
		err->ShowModal();
    }
}

void MyFrame::unloadConfig()
{
	unloadAll<Room>();
	unloadAll<OverlayType>();
	unloadAll<ActorType>();
	unloadAll<Sprite>();
	unloadAll<Material>();
}

void MyFrame::saveConfig()
{
	saveAll<Material>(currentPath.ToStdString());
	saveAll<Sprite>(currentPath.ToStdString());
	saveAll<ActorType>(currentPath.ToStdString());
	saveAll<OverlayType>(currentPath.ToStdString());
	saveAll<Room>(currentPath.ToStdString());
}

void MyFrame::reloadConfig()
{
    unloadConfig();
    loadConfig();
}


// menu command handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	unloadConfig();
	Close(true);
}
void MyFrame::OnNew(wxCommandEvent& event) 
{
	wxString p = wxGetCwd();
	wxString f;
	wxTextEntryDialog *projName = new wxTextEntryDialog(this, "Enter Project Name: ");
	if (projName->ShowModal() == wxID_OK)
	{
		f = projName->GetValue();
	}
	if (f != wxEmptyString)
	{
		f;
		wxMkdir(f);
		currentPath = p + "//" + f;
		if (wxSetWorkingDirectory(currentPath))
		{
			wxMkdir("actor_types");
			wxMkdir("materials");
			wxMkdir("overlay_types");
			wxMkdir("rooms");
			wxMkdir("audio");
			wxMkdir("sprites");
			wxSetWorkingDirectory(p);
		}
	}
	else
	{
		wxMessageDialog* err = new wxMessageDialog(this, "Must enter a project name. Please try again.", "ERROR", wxICON_ERROR | wxOK | wxCENTRE);
		err->ShowModal();
	}
}
void MyFrame::OnOpen(wxCommandEvent& event) 
{
	wxDirDialog *openProjDialog = new wxDirDialog(this, "Select Project Directory");
	if (openProjDialog->ShowModal() == wxID_OK) {
		wxString fileName = openProjDialog->GetPath();
		currentPath = fileName;
	}

	loadConfig();
}
void MyFrame::OnSave(wxCommandEvent& event)
{
	saveConfig();
}
void MyFrame::OnPlay(wxCommandEvent& WXUNUSED(event))
{
	reloadConfig();
	try
	{
		Engine::getInstance().run();
	}
	catch (const ConfigurationError& e)
	{
		std::cerr << e.what() << std::endl;
		wxMessageDialog* err = new wxMessageDialog(this, e.what(), "ERROR", wxICON_ERROR | wxOK | wxCENTRE);
		err->ShowModal();
	}
}

// MySplitterWindow

MySplitterWindow::MySplitterWindow(wxFrame *parent)
	: wxSplitterWindow(parent, wxID_ANY,
		wxDefaultPosition, wxDefaultSize,
		wxSP_3D | wxSP_LIVE_UPDATE |
		wxCLIP_CHILDREN)
{
	m_frame = parent;
}

void MySplitterWindow::OnChange()
{
	MyFrame* f = (MyFrame*)m_frame;
	Editor* e = (Editor*)f->m_right;
	e->resetTrigger();
}

//Sidebar Window
Sidebar::Sidebar(wxWindow* parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetScrollbars(20, 20, 5, 5);
	this->parent = parent;
	wxBoxSizer *vszr = new wxBoxSizer(wxVERTICAL);
	wxGridSizer *buttons = new wxGridSizer(6, 1, 20, 20);
	wxPanel *pnl = new wxPanel(this, wxID_ANY);
	wxButton *bt1 = new wxButton(pnl, SPRITE, "Sprites");
	wxButton *bt2 = new wxButton(pnl, AUDIO, "Audio");
	wxButton *bt3 = new wxButton(pnl, ROOM, "Rooms");
	wxButton *bt4 = new wxButton(pnl, OBJECT, "Objects");
	wxButton *bt5 = new wxButton(pnl, OVERLAY, "Overlays");
	wxButton *bt6 = new wxButton(pnl, MATERIAL, "Materials");

	Connect(SPRITE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onSprite));
	Connect(AUDIO, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onAudio));
	Connect(ROOM, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onRoom));
	Connect(OBJECT, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onObject));
	Connect(OVERLAY, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onOverlay));
	Connect(MATERIAL, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onMaterial));

	buttons->Add(bt1, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt2, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt3, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt4, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt5, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt6, 0, wxALIGN_CENTER | wxCENTER, 20);

	pnl->SetSizer(buttons);

	vszr->Add(pnl, 1, wxEXPAND);

	this->SetSizer(vszr);

	this->SetFocus();
	pnl->SetFocus();
	Centre();
}

void Sidebar::onSprite(wxCommandEvent& event)
{
	operation = "sprites";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Sprite Control", wxDefaultPosition, wxSize(270, 200));
	
	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : Sprite::objectMap)
	{
		listbox->Append(pair.first);
	}

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onAudio(wxCommandEvent& event)
{
	operation = "audio";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Audio Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	//Audio map

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onObject(wxCommandEvent& event)
{
	operation = "actor_types";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Object Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : ActorType::objectMap)
	{
		listbox->Append(pair.first);
	}

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onRoom(wxCommandEvent& event)
{
	operation = "rooms";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Room Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : Room::objectMap)
	{
		listbox->Append(pair.first);
	}

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onMaterial(wxCommandEvent& event)
{
	operation = "materials";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Material Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : Material::objectMap)
	{
		listbox->Append(pair.first);
	}

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onOverlay(wxCommandEvent& event)
{
	operation = "overlay_types";
	wxFrame *boxFrame = new wxFrame(NULL, wxID_ANY, "Overlay Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : OverlayType::objectMap)
	{
		listbox->Append(pair.first);
	}

	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

	listbox->Bind(wxEVT_LISTBOX, &Sidebar::onSelect, this);

	gbox->Add(newBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	gbox->Add(delBtn, 0, wxALIGN_CENTER | wxCENTER, 20);
	btnPanel->SetSizer(gbox);

	hbox->Add(btnPanel, 2, wxEXPAND | wxRIGHT, 10);
	panel->SetSizer(hbox);
	this->SetFocus();
	boxFrame->SetFocus();
	panel->SetFocus();
	btnPanel->SetFocus();

	boxFrame->Show(true);
}
void Sidebar::onNew(wxCommandEvent& WXUNUSED(event))
{
	wxString str;
	wxTextEntryDialog* getTxt = new wxTextEntryDialog(this, "Enter New " + operation + " Name");
	if (getTxt->ShowModal() == wxID_OK)
	{
		str = getTxt->GetValue();
	}
	if (str.Len() > 0)
	{
		listbox->Append(str);
		if (operation == "sprites")
		{
			Sprite::createSprite(str.ToStdString());
		}
		else if (operation == "actor_types")
		{
			ActorType::createActorType(str.ToStdString());
		}
		else if (operation == "overlay_types")
		{
			OverlayType::createOverlayType(str.ToStdString());
		}
		else if (operation == "materials")
		{
			Material::createMaterial(str.ToStdString());
		}
		else if (operation == "rooms")
		{
			Room::createRoom(str.ToStdString());
		}
	}
}
void Sidebar::onSelect(wxCommandEvent& event)
{
	int sel = listbox->GetSelection();
	if (sel != -1)
	{
		wxString str = listbox->GetString(sel);
		selObject = str.ToStdString();
		MySplitterWindow* p = (MySplitterWindow*) this->GetParent();
		p->OnChange();
	}
}
void Sidebar::onDelete(wxCommandEvent& event)
{
	int sel = listbox->GetSelection();
	if (sel != -1) 
	{
		wxString str = listbox->GetString(sel);
		listbox->Delete(sel);
		if (operation == "sprites")
		{
			Sprite::objectMap.erase(str.ToStdString());
		}
		else if (operation == "actor_types")
		{
			ActorType::objectMap.erase(str.ToStdString());
		}
		else if (operation == "overlay_types")
		{
			OverlayType::objectMap.erase(str.ToStdString());
		}
		else if (operation == "materials")
		{
			Material::objectMap.erase(str.ToStdString());
		}
		else if (operation == "rooms")
		{
			Room::objectMap.erase(str.ToStdString());
		}
	}
}

//Editor Window
Editor::Editor(wxWindow* parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetScrollbars(20, 20, 5, 5);
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxPanel *pnl1, *pnl2, *pnl3, *pnl4;
	wxBoxSizer *vszr = new wxBoxSizer(wxVERTICAL);
	wxGridSizer* obGrid = new wxGridSizer(2, 2, 5, 5);

	pnl1 = new wxPanel(panel, wxID_ANY);
	pnl2 = new wxPanel(panel, wxID_ANY);
	pnl3 = new wxPanel(panel, wxID_ANY);
	pnl4 = new wxPanel(panel, wxID_ANY);

	wxBoxSizer* bx1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bx2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bx3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bx4 = new wxBoxSizer(wxHORIZONTAL);
	lb1 = new wxListBox(pnl1, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb2 = new wxListBox(pnl2, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb3 = new wxListBox(pnl3, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb4 = new wxListBox(pnl4, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));

	lb1->Bind(wxEVT_LISTBOX, &Editor::onBox1Select, this);

	bx1->Add(lb1, 3, wxEXPAND | wxALL, 2);
	bx2->Add(lb2, 3, wxEXPAND | wxALL, 2);
	bx3->Add(lb3, 3, wxEXPAND | wxALL, 2);
	bx4->Add(lb4, 3, wxEXPAND | wxALL, 2);

	wxPanel *bpnl1 = new wxPanel(pnl1, wxID_ANY);
	wxPanel *bpnl2 = new wxPanel(pnl2, wxID_ANY);
	wxPanel *bpnl3 = new wxPanel(pnl3, wxID_ANY);
	wxPanel *bpnl4 = new wxPanel(pnl4, wxID_ANY);

	wxGridSizer* gdbox1 = new wxGridSizer(3, 1, 5, 5);
	wxGridSizer* gdbox2 = new wxGridSizer(3, 1, 5, 5);
	wxGridSizer* gdbox3 = new wxGridSizer(3, 1, 5, 5);
	wxGridSizer* gdbox4 = new wxGridSizer(3, 1, 5, 5);

	wxButton* newBtn1 = new wxButton(bpnl1, NEW_ITEM, wxT("New"));
	wxButton* editBtn1 = new wxButton(bpnl1, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn1 = new wxButton(bpnl1, DELETE_ITEM, wxT("Delete"));
	newBtn1->Bind(wxEVT_BUTTON, &Editor::onNew, this);
	editBtn1->Bind(wxEVT_BUTTON, &Editor::onEdit, this);
	delBtn1->Bind(wxEVT_BUTTON, &Editor::onDelete, this);

	wxButton* newBtn2 = new wxButton(bpnl2, NEW_ITEM, wxT("New"));
	wxButton* editBtn2 = new wxButton(bpnl2, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn2 = new wxButton(bpnl2, DELETE_ITEM, wxT("Delete"));
	newBtn2->Bind(wxEVT_BUTTON, &Editor::onNew, this);
	editBtn2->Bind(wxEVT_BUTTON, &Editor::onEdit, this);
	delBtn2->Bind(wxEVT_BUTTON, &Editor::onDelete, this);

	wxButton* newBtn3 = new wxButton(bpnl3, NEW_ITEM, wxT("New"));
	wxButton* editBtn3 = new wxButton(bpnl3, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn3 = new wxButton(bpnl3, DELETE_ITEM, wxT("Delete"));
	newBtn3->Bind(wxEVT_BUTTON, &Editor::onNew, this);
	editBtn3->Bind(wxEVT_BUTTON, &Editor::onEdit, this);
	delBtn3->Bind(wxEVT_BUTTON, &Editor::onDelete, this);

	wxButton* newBtn4 = new wxButton(bpnl4, NEW_ITEM, wxT("New"));
	wxButton* editBtn4 = new wxButton(bpnl4, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn4 = new wxButton(bpnl4, DELETE_ITEM, wxT("Delete"));
	newBtn4->Bind(wxEVT_BUTTON, &Editor::onNew, this);
	editBtn4->Bind(wxEVT_BUTTON, &Editor::onEdit, this);
	delBtn4->Bind(wxEVT_BUTTON, &Editor::onDelete, this);

	gdbox1->Add(newBtn1, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox1->Add(editBtn1, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox1->Add(delBtn1, 0, wxALIGN_CENTER | wxCENTER, 2);
	bpnl1->SetSizer(gdbox1);

	gdbox2->Add(newBtn2, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox2->Add(editBtn2, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox2->Add(delBtn2, 0, wxALIGN_CENTER | wxCENTER, 2);
	bpnl2->SetSizer(gdbox2);

	gdbox3->Add(newBtn3, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox3->Add(editBtn3, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox3->Add(delBtn3, 0, wxALIGN_CENTER | wxCENTER, 2);
	bpnl3->SetSizer(gdbox3);

	gdbox4->Add(newBtn4, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox4->Add(editBtn4, 0, wxALIGN_CENTER | wxCENTER, 2);
	gdbox4->Add(delBtn4, 0, wxALIGN_CENTER | wxCENTER, 2);
	bpnl4->SetSizer(gdbox4);

	bx1->Add(bpnl1, 2, wxEXPAND | wxRIGHT, 2);
	bx2->Add(bpnl2, 2, wxEXPAND | wxRIGHT, 2);
	bx3->Add(bpnl3, 2, wxEXPAND | wxRIGHT, 2);
	bx4->Add(bpnl4, 2, wxEXPAND | wxRIGHT, 2);

	pnl1->SetSizer(bx1);
	pnl2->SetSizer(bx2);
	pnl3->SetSizer(bx3);
	pnl4->SetSizer(bx4);

	obGrid->Add(pnl1, 0, wxEXPAND | wxTOP | wxLEFT, 2);
	obGrid->Add(pnl2, 0, wxEXPAND | wxTOP | wxRIGHT, 2);
	obGrid->Add(pnl3, 0, wxEXPAND | wxBOTTOM | wxLEFT, 2);
	obGrid->Add(pnl4, 0, wxEXPAND | wxBOTTOM | wxRIGHT, 2);

	panel->SetSizer(obGrid);
	vszr->Add(panel, 1, wxEXPAND);

	this->SetSizer(vszr);
}

void Editor::resetTrigger()
{
	lb1->Clear();
	lb2->Clear();
	lb3->Clear();
	lb4->Clear();
	if (operation == "actor_types")
	{
		ActorType* at = ActorType::objectMap.at(selObject);
		std::map<const Trigger*, std::list<Action*>, TriggerComparator> actionMap = at->actionMap;
		for (const auto& pair : actionMap)
		{
			lb1->Append(pair.first->getTypeName());
		}
	}
}
void Editor::resetAction()
{
	lb2->Clear();
	lb3->Clear();
	
}
void Editor::resetCon()
{

}

void Editor::onNew(wxCommandEvent& event)
{

}
void Editor::onEdit(wxCommandEvent& event)
{

}
void Editor::onDelete(wxCommandEvent& event)
{

}

void Editor::onBox1Select(wxCommandEvent& event)
{
	int sel = lb1->GetSelection();
	if (sel != -1)
	{
		wxString str = lb1->GetString(sel);
		selTrigger = str.ToStdString();
		resetAction();
	}
}