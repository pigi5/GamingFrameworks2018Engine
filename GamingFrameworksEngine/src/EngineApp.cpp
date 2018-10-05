#include "wx/wxprec.h"
#include "wx/splitter.h"
#include "wx/dcmirror.h"
#include "wx/dir.h"
#include "wx/file.h"
#include "../header/Engine.h"
#include <direct.h>
#include <iostream>

string operation;
wxListBox* listbox;
wxString currentPath;

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
	DELETE_ITEM = 12,
	CLEAR = 13,
	PLAY = 14
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

private:
	wxWindow *m_left, *m_right;

	wxSplitterWindow* m_splitter;
	wxWindow *m_replacewindow;

	wxDECLARE_EVENT_TABLE();
};

class MySplitterWindow : public wxSplitterWindow
{
public:
	MySplitterWindow(wxFrame *parent);

private:
	wxFrame *m_frame;
};

class Sidebar : public wxScrolledWindow
{
public:
	Sidebar(wxWindow* parent);
	virtual ~Sidebar() {};

	void onSprite(wxCommandEvent& event);
	void onAudio(wxCommandEvent& event);
	void onObject(wxCommandEvent& event);
	void onRoom(wxCommandEvent& event);
	void onMaterial(wxCommandEvent& event);
	void onOverlay(wxCommandEvent& event);
	void onNew(wxCommandEvent& event);
	void onDelete(wxCommandEvent& event);

private:
};

class Editor : public wxScrolledWindow
{
public:
	Editor(wxWindow* parent);
	virtual ~Editor() {};
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

	m_splitter->SplitVertically(m_left, m_right, this->GetSize().GetX()/4);

	m_replacewindow = NULL;
}

MyFrame::~MyFrame()
{
	if (m_replacewindow) {
		m_replacewindow->Destroy();
	}
}

// menu command handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
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
}
void MyFrame::OnSave(wxCommandEvent& event)
{
	Close(true);
}
void MyFrame::OnPlay(wxCommandEvent& WXUNUSED(event))
{
	try
	{
		Engine::getInstance().run();
	}
	catch (const ConfigurationError& e)
	{
		std::cout << e.what() << std::endl;
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

//Sidebar Window

Sidebar::Sidebar(wxWindow* parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetScrollbars(20, 20, 5, 5);
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
	wxDir* directory = new wxDir(currentPath + "//sprites");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxDir* directory = new wxDir(currentPath + "//audio");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxDir* directory = new wxDir(currentPath + "//actor_types");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxDir* directory = new wxDir(currentPath + "//rooms");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxDir* directory = new wxDir(currentPath + "//materials");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxDir* directory = new wxDir(currentPath + "//overlay_types");
	if (directory->IsOpened() && directory->HasFiles())
	{
		wxArrayString* filenames = new wxArrayString();
		directory->GetAllFiles(directory->GetName(), filenames);
		for (int i = 0; i < filenames->GetCount(); i++)
		{
			wxString fn = wxFileNameFromPath(filenames->Item(i));
			listbox->Append(fn.BeforeLast('.'));
		}
	}
	hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

	wxPanel *btnPanel = new wxPanel(panel, wxID_ANY);
	wxGridSizer* gbox = new wxGridSizer(2, 1, 20, 20);

	wxButton* newBtn = new wxButton(btnPanel, NEW_ITEM, wxT("New"));
	newBtn->Bind(wxEVT_BUTTON, &Sidebar::onNew, this);
	wxButton* delBtn = new wxButton(btnPanel, DELETE_ITEM, wxT("Delete"));
	delBtn->Bind(wxEVT_BUTTON, &Sidebar::onDelete, this);

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
	wxTextEntryDialog* getTxt = new wxTextEntryDialog(this, "Enter New Sprite Name");
	if (getTxt->ShowModal() == wxID_OK)
	{
		str = getTxt->GetValue();
	}
	if (str.Len() > 0)
	{
		listbox->Append(str);
		wxSetWorkingDirectory(currentPath + "//" + operation);
		wxFile* nf = new wxFile();
		nf->Create(str + ".yaml");
		nf->Close();
		wxSetWorkingDirectory(currentPath);
	}
}
void Sidebar::onDelete(wxCommandEvent& event)
{
	int sel = listbox->GetSelection();
	if (sel != -1) {
		wxSetWorkingDirectory(currentPath + "//" + operation);
		wxString str = listbox->GetString(sel);
		wxRemoveFile(str + ".yaml");
		wxSetWorkingDirectory(currentPath);
		listbox->Delete(sel);
	}
}

//Editor Window
Editor::Editor(wxWindow* parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetScrollbars(20, 20, 5, 5);
}