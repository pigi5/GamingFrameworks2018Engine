#include "wx/wxprec.h"
#include "wx/splitter.h"
#include "wx/dcmirror.h"
#include "wx/dir.h"
#include "wx/file.h"
#include "../header/Engine.h"
#include "../header/Music.h"
#include "../header/Sound.h"
#include "../header/Utils.h"
#include "../header/MouseStates.h"
#include "../header/KeyMap.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"
#include <direct.h>
#include <iostream>

int operation;
wxListBox* listbox;
wxFrame* boxFrame;
wxString currentPath;
string selObject;
string selTrigger;
string selAction;
wxStaticText* topText;
wxStaticText* st1;
wxStaticText* st2;
wxStaticText* st3;
wxStaticText* st4;

wxString DEFAULT_TOP_STR("Edit Panel: Select an object to edit from the sidebar.");
wxString UNOPENED_TOP_STR("Open a project to start editing.");

// ID for the menu commands
enum
{
	QUIT = 1,
	NEW = 2,
	OPEN = 3,
	SAVE = 4,
	ROOM = 5,
	ACTOR = 6,
	SPRITE = 7,
	SOUND = 8,
	MUSIC = 16,
	OVERLAY = 10,
	ATTRIBUTE = 9,
	NEW_ITEM = 11,
	EDIT_ITEM = 12,
	DELETE_ITEM = 13,
	CLEAR = 14,
	PLAY = 15,
	DEF = 16,
	FOL = 17,
	SELOBJ = 18
};

unordered_map<int, std::string> actionNames;

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
	void onSound(wxCommandEvent& event);
	void onMusic(wxCommandEvent& event);
	void onActor(wxCommandEvent& event);
	void onRoom(wxCommandEvent& event);
	void onAttribute(wxCommandEvent& event);
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
	
	void onNew1(wxCommandEvent& event);
	void onEdit1(wxCommandEvent& event);
	void onDelete1(wxCommandEvent& event);

	void onNew2(wxCommandEvent& event);
	void onEdit2(wxCommandEvent& event);
	void onDelete2(wxCommandEvent& event);

	void onNew3(wxCommandEvent& event);
	void onEdit3(wxCommandEvent& event);
	void onDelete3(wxCommandEvent& event);

	void onNew4(wxCommandEvent& event);
	void onEdit4(wxCommandEvent& event);
	void onDelete4(wxCommandEvent& event);

	void onBox1Select(wxCommandEvent& event);
	void onBox2Select(wxCommandEvent& event);

	wxListBox* lb1;
	wxListBox* lb2;
	wxListBox* lb3;
	wxListBox* lb4;
};

class RoomEditor : public wxFrame
{
public:
	Room* rm;

	RoomEditor();

	wxScrolledWindow* rmEdit;
	wxStaticText* defTxt;
	wxStaticText* folTxt;
	wxStaticText* actTxt;
	wxStaticText* ovTxt;

	wxListBox* actBox;
	wxListBox* ovBox;

	void onSetDefault(wxCommandEvent& event);
	void onSetFollow(wxCommandEvent& event);

	void onActNew(wxCommandEvent& event);
	void onActEdit(wxCommandEvent& event);
	void onActDelete(wxCommandEvent& event);

	void onOvNew(wxCommandEvent& event);
	void onOvEdit(wxCommandEvent& event);
	void onOvDelete(wxCommandEvent& event);
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
    
    actionNames[SPRITE] = "Sprite";
    actionNames[ACTOR] = "Actor";
    actionNames[OVERLAY] = "Overlay";
    actionNames[ROOM] = "Room";
    actionNames[SOUND] = "Sound";
    actionNames[MUSIC] = "Music";
    actionNames[ATTRIBUTE] = "Attribute";

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
	: wxFrame(NULL, wxID_ANY, wxT("RAGE"),
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
        std::string currentDir = currentPath.ToStdString();

        Engine::loadAttributes(currentDir);
	    loadAll<Sprite>(currentDir);
		loadAll<Sound>(currentDir);
		loadAll<Music>(currentDir);
	    // load shallow first so we can have all the name references
	    loadAll<ActorType>(currentDir, true);
	    loadAll<ActorType>(currentDir);
	    // load shallow first so we can have all the name references
	    loadAll<OverlayType>(currentDir, true);
	    loadAll<OverlayType>(currentDir);
	    loadAll<Room>(currentDir);
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
    Engine::defaultAttributes.clear();
	unloadAll<Room>();
	unloadAll<OverlayType>();
	unloadAll<ActorType>();
	unloadAll<Sprite>();
	unloadAll<Music>();
	unloadAll<Sound>();
}

void MyFrame::saveConfig()
{
    std::string currentDir = currentPath.ToStdString();

    Engine::saveAttributes(currentDir);
	saveAll<Sprite>(currentDir);
	saveAll<Sound>(currentDir);
	saveAll<Music>(currentDir);
	saveAll<ActorType>(currentDir);
	saveAll<OverlayType>(currentDir);
	saveAll<Room>(currentDir);
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
		this->SetLabel("RAGE - " + f);
		wxMkdir(f);
		currentPath = p + "//" + f;
		if (wxSetWorkingDirectory(currentPath))
		{
			wxMkdir("actor_types");
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
		this->SetLabel("RAGE - " + fileName.AfterLast('\\'));
        topText->SetLabel(DEFAULT_TOP_STR);
		reloadConfig();
	}
}
void MyFrame::OnSave(wxCommandEvent& event)
{
	saveConfig();
}
void MyFrame::OnPlay(wxCommandEvent& WXUNUSED(event))
{
	if (currentPath != wxEmptyString)
	{
		saveConfig();
		reloadConfig();
		try
		{
			Engine::getInstance().run();
            // avoid problems if instance are created at runtime
            reloadConfig();
		}
		catch (const ConfigurationError& e)
		{
			std::cerr << e.what() << std::endl;
			wxMessageDialog* err = new wxMessageDialog(this, e.what(), "ERROR", wxICON_ERROR | wxOK | wxCENTRE);
			err->ShowModal();
		}
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
	wxGridSizer *buttons = new wxGridSizer(7, 1, 20, 20);
	wxPanel *pnl = new wxPanel(this, wxID_ANY);
	wxButton *bt1 = new wxButton(pnl, SPRITE, "Sprites");
	wxButton *bt2 = new wxButton(pnl, SOUND, "Sound");
	wxButton *bt7 = new wxButton(pnl, MUSIC, "Music");
	wxButton *bt3 = new wxButton(pnl, ROOM, "Rooms");
	wxButton *bt4 = new wxButton(pnl, ACTOR, "Actors");
	wxButton *bt5 = new wxButton(pnl, OVERLAY, "Overlays");
	wxButton *bt6 = new wxButton(pnl, ATTRIBUTE, "Attributes");

	Connect(SPRITE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onSprite));
	Connect(SOUND, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onSound));
	Connect(MUSIC, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onMusic));
	Connect(ROOM, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onRoom));
	Connect(ACTOR, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onActor));
	Connect(OVERLAY, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onOverlay));
	Connect(ATTRIBUTE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onAttribute));

	buttons->Add(bt1, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt2, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt7, 0, wxALIGN_CENTER | wxCENTER, 20);
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
	operation = SPRITE;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Sprite Control", wxDefaultPosition, wxSize(270, 200));
	
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
void Sidebar::onSound(wxCommandEvent& event)
{
	operation = SOUND;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Sound Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : Sound::objectMap)
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
void Sidebar::onMusic(wxCommandEvent& event)
{
	operation = MUSIC;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Music Control", wxDefaultPosition, wxSize(270, 200));

	wxPanel * panel = new wxPanel(boxFrame, -1);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	listbox = new wxListBox(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	
	for (const auto& pair : Music::objectMap)
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
void Sidebar::onActor(wxCommandEvent& event)
{
	operation = ACTOR;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Actor Control", wxDefaultPosition, wxSize(270, 200));

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
	operation = ROOM;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Room Control", wxDefaultPosition, wxSize(270, 200));

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

void Sidebar::onOverlay(wxCommandEvent& event)
{
	operation = OVERLAY;
	boxFrame = new wxFrame(NULL, wxID_ANY, "Overlay Control", wxDefaultPosition, wxSize(270, 200));

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

void Sidebar::onAttribute(wxCommandEvent& event)
{
	operation = ATTRIBUTE;
	selObject = "";
	MySplitterWindow* p = (MySplitterWindow*) this->GetParent();
	p->OnChange();
}

void Sidebar::onNew(wxCommandEvent& WXUNUSED(event))
{
	wxString str, fn;
	wxTextEntryDialog* getTxt = new wxTextEntryDialog(this, "Enter New " + actionNames[operation] + " Name");
	if (getTxt->ShowModal() == wxID_OK)
	{
		str = getTxt->GetValue();
	}
	if (str.Len() > 0)
	{
		listbox->Append(str);
        switch (operation)
        {
            case SPRITE:
			    Sprite::createSprite(str.ToStdString());
                break;
            case ACTOR:
			    ActorType::createActorType(str.ToStdString());
                break;
            case OVERLAY:
			    OverlayType::createOverlayType(str.ToStdString());
                break;
            case ROOM:
			    Room::createRoom(str.ToStdString());
                break;
            case SOUND:
            {
			    wxFileDialog *openSndDialog = new wxFileDialog(this, "Select Sound File");
			    openSndDialog->SetWildcard("WAV files (*.wav)|*.wav");
			    if (openSndDialog->ShowModal() == wxID_OK) {
				    fn= openSndDialog->GetPath();
				    wxCopyFile(fn, currentPath + "\\sounds\\" + fn.AfterLast('\\'));
				    wxString relPath = wxString(currentPath);
				    relPath = relPath.AfterLast('\\');
				    relPath += "\\sounds\\" + fn.AfterLast('\\');
				    Sound::createSound(str.ToStdString(), relPath.ToStdString());
				    boxFrame->Close(true);
			    }
                break;
            }
            case MUSIC:
            {
			    wxFileDialog *openMusDialog = new wxFileDialog(this, "Select Music File");
			    openMusDialog->SetWildcard("WAV files (*.wav)|*.wav");
			    if (openMusDialog->ShowModal() == wxID_OK) {
				    fn = openMusDialog->GetPath();
				    wxCopyFile(fn, currentPath + "\\music\\" + fn.AfterLast('\\'));
				    wxString relPath = wxString(currentPath);
				    relPath = relPath.AfterLast('\\');
				    relPath += "\\music\\" + fn.AfterLast('\\');
				    Music::createMusic(str.ToStdString(), relPath.ToStdString());
				    boxFrame->Close(true);
			    }
                break;
            }
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
        switch (operation)
        {
            case SPRITE:
			    Sprite::objectMap.erase(str.ToStdString());
				wxRemoveFile(currentPath + "\\sprites\\" + str + ".yml");
                break;
            case ACTOR:
			    ActorType::objectMap.erase(str.ToStdString());
				wxRemoveFile(currentPath + "\\actor_types\\" + str + ".yml");
                break;
            case OVERLAY:
			    OverlayType::objectMap.erase(str.ToStdString());
				wxRemoveFile(currentPath + "\\overlay_types\\" + str + ".yml");
                break;
            case ROOM:
			    Room::objectMap.erase(str.ToStdString());
				wxRemoveFile(currentPath + "\\rooms\\" + str + ".yml");
                break;
            case SOUND:
			    Sound::objectMap.erase(str.ToStdString());
			    wxRemoveFile(currentPath + "\\sounds\\" + str + ".yml");
                break;
            case MUSIC:
			    Music::objectMap.erase(str.ToStdString());
			    wxRemoveFile(currentPath + "\\music\\" + str + ".yml");
                break;
        }
		boxFrame->Close(true);
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
	wxGridSizer* obGrid = new wxGridSizer(2, 2, 2, 2);

	pnl1 = new wxPanel(panel, wxID_ANY);
	pnl2 = new wxPanel(panel, wxID_ANY);
	pnl3 = new wxPanel(panel, wxID_ANY);
	pnl4 = new wxPanel(panel, wxID_ANY);

	wxBoxSizer* bx1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bx2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bx3 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bx4 = new wxBoxSizer(wxVERTICAL);
	lb1 = new wxListBox(pnl1, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb2 = new wxListBox(pnl2, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb3 = new wxListBox(pnl3, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	lb4 = new wxListBox(pnl4, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));

	lb1->Bind(wxEVT_LISTBOX, &Editor::onBox1Select, this);
	lb2->Bind(wxEVT_LISTBOX, &Editor::onBox2Select, this);

	wxPanel *bpnl1 = new wxPanel(pnl1, wxID_ANY);
	wxPanel *bpnl2 = new wxPanel(pnl2, wxID_ANY);
	wxPanel *bpnl3 = new wxPanel(pnl3, wxID_ANY);
	wxPanel *bpnl4 = new wxPanel(pnl4, wxID_ANY);

	wxGridSizer* gdbox1 = new wxGridSizer(1, 3, 2, 2);
	wxGridSizer* gdbox2 = new wxGridSizer(1, 3, 2, 2);
	wxGridSizer* gdbox3 = new wxGridSizer(1, 3, 2, 2);
	wxGridSizer* gdbox4 = new wxGridSizer(1, 3, 2, 2);

	wxButton* newBtn1 = new wxButton(bpnl1, NEW_ITEM, wxT("New"));
	wxButton* editBtn1 = new wxButton(bpnl1, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn1 = new wxButton(bpnl1, DELETE_ITEM, wxT("Delete"));
	newBtn1->Bind(wxEVT_BUTTON, &Editor::onNew1, this);
	editBtn1->Bind(wxEVT_BUTTON, &Editor::onEdit1, this);
	delBtn1->Bind(wxEVT_BUTTON, &Editor::onDelete1, this);

	wxButton* newBtn2 = new wxButton(bpnl2, NEW_ITEM, wxT("New"));
	wxButton* editBtn2 = new wxButton(bpnl2, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn2 = new wxButton(bpnl2, DELETE_ITEM, wxT("Delete"));
	newBtn2->Bind(wxEVT_BUTTON, &Editor::onNew2, this);
	editBtn2->Bind(wxEVT_BUTTON, &Editor::onEdit2, this);
	delBtn2->Bind(wxEVT_BUTTON, &Editor::onDelete2, this);

	wxButton* newBtn3 = new wxButton(bpnl3, NEW_ITEM, wxT("New"));
	wxButton* editBtn3 = new wxButton(bpnl3, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn3 = new wxButton(bpnl3, DELETE_ITEM, wxT("Delete"));
	newBtn3->Bind(wxEVT_BUTTON, &Editor::onNew3, this);
	editBtn3->Bind(wxEVT_BUTTON, &Editor::onEdit3, this);
	delBtn3->Bind(wxEVT_BUTTON, &Editor::onDelete3, this);

	wxButton* newBtn4 = new wxButton(bpnl4, NEW_ITEM, wxT("New"));
	wxButton* editBtn4 = new wxButton(bpnl4, EDIT_ITEM, wxT("Edit"));
	wxButton* delBtn4 = new wxButton(bpnl4, DELETE_ITEM, wxT("Delete"));
	newBtn4->Bind(wxEVT_BUTTON, &Editor::onNew4, this);
	editBtn4->Bind(wxEVT_BUTTON, &Editor::onEdit4, this);
	delBtn4->Bind(wxEVT_BUTTON, &Editor::onDelete4, this);

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

	st1 = new wxStaticText(pnl1, wxID_ANY, "Primary", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE);
	st2 = new wxStaticText(pnl2, wxID_ANY, "Secondary", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE);
	st3 = new wxStaticText(pnl3, wxID_ANY, "Tertiary", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE);
	st4 = new wxStaticText(pnl4, wxID_ANY, "Quaternary", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE);

	bx1->Add(st1, 0, wxEXPAND | wxCENTRE | wxALL, 7);
	bx2->Add(st2, 0, wxEXPAND | wxCENTRE | wxALL, 7);
	bx3->Add(st3, 0, wxEXPAND | wxCENTRE | wxALL, 7);
	bx4->Add(st4, 0, wxEXPAND | wxCENTRE | wxALL, 7);

	bx1->Add(bpnl1, 0, wxEXPAND | wxBOTTOM, 5);
	bx2->Add(bpnl2, 0, wxEXPAND | wxBOTTOM, 5);
	bx3->Add(bpnl3, 0, wxEXPAND | wxBOTTOM, 5);
	bx4->Add(bpnl4, 0, wxEXPAND | wxBOTTOM, 5);

	bx1->Add(lb1, 1, wxEXPAND | wxALL, 2);
	bx2->Add(lb2, 1, wxEXPAND | wxALL, 2);
	bx3->Add(lb3, 1, wxEXPAND | wxALL, 2);
	bx4->Add(lb4, 1, wxEXPAND | wxALL, 2);

	pnl1->SetSizer(bx1);
	pnl2->SetSizer(bx2);
	pnl3->SetSizer(bx3);
	pnl4->SetSizer(bx4);

	obGrid->Add(pnl1, 0, wxEXPAND | wxTOP | wxLEFT, 2);
	obGrid->Add(pnl2, 0, wxEXPAND | wxTOP | wxRIGHT, 2);
	obGrid->Add(pnl3, 0, wxEXPAND | wxBOTTOM | wxLEFT, 2);
	obGrid->Add(pnl4, 0, wxEXPAND | wxBOTTOM | wxRIGHT, 2);

    topText = new wxStaticText(this, wxID_ANY, UNOPENED_TOP_STR, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE);
   
	panel->SetSizer(obGrid);
	vszr->Add(topText, 0, wxEXPAND | wxTOP, 10);
	vszr->Add(panel, 1, wxEXPAND);

	this->SetSizer(vszr);
}

void Editor::resetTrigger()
{
	lb1->Clear();
	lb2->Clear();
	lb3->Clear();
	lb4->Clear();
    
    switch (operation)
    {
        case SPRITE:
        {
            topText->SetLabel(wxString("Now Editing " + actionNames[operation] + " - " + selObject));
            st1->SetLabel("Images");
		    st2->SetLabel("Unused");
		    st3->SetLabel("Unused");
		    st4->SetLabel("Unused");

		    Sprite* spr = Sprite::objectMap.at(selObject);
		    vector<string> files = spr->textrFiles;
		    for (int i = 0; i < files.size(); i++)
		    {
			    wxString str = wxString(files[i]);
			    lb1->Append(str.AfterLast('\\'));
		    }
		    boxFrame->Close(true);
            break;
        }
        case ACTOR:
        case OVERLAY:
        {
            topText->SetLabel(wxString("Now Editing " + actionNames[operation] + " - " + selObject));
		    st1->SetLabel("Triggers");
		    st2->SetLabel("Actions");
		    st3->SetLabel("Conditionals");
		    st4->SetLabel("Attributes");

		    ActorType* at;
            if (operation == ACTOR)
            {
                at = ActorType::objectMap.at(selObject);
            } 
            else
            {
                at = OverlayType::objectMap.at(selObject);
            }
		    std::unordered_map<Trigger*, std::list<Action*>, TriggerHash, TriggerEquals> actionMap = at->actionMap;
		    for (const auto& pair : actionMap)
		    {
			    string str = pair.first->toString();
			    lb1->Append(str);
		    }
		    std::unordered_map<std::string, int> attributes = at->attributes;
		    for (const auto& pair : attributes)
		    {
			    string str = pair.first + ": {default: " + to_string(pair.second) + "}";
			    lb4->Append(str);
		    }
			string str;
			if (at->sprite != nullptr)
			{
				str = "Sprite: {" + at->sprite->name + "}";
			}
			else {
				str = "Sprite: {None}";
			}
			lb4->Append(str);
		    boxFrame->Close(true);
            break;
        }
        case ROOM:
        {
            topText->SetLabel(DEFAULT_TOP_STR);
		    st1->SetLabel("Unused");
		    st2->SetLabel("Unused");
		    st3->SetLabel("Unused");
		    st4->SetLabel("Unused");

		    RoomEditor* rmEditor = new RoomEditor();
		    rmEditor->Show(true);
		    boxFrame->Close(true);
            break;
        }
        case SOUND:
            topText->SetLabel(DEFAULT_TOP_STR);
		    st1->SetLabel("Unused");
		    st2->SetLabel("Unused");
		    st3->SetLabel("Unused");
		    st4->SetLabel("Unused");
            break;
        case MUSIC:
            topText->SetLabel(DEFAULT_TOP_STR);
		    st1->SetLabel("Unused");
		    st2->SetLabel("Unused");
		    st3->SetLabel("Unused");
		    st4->SetLabel("Unused");
            break;
        case ATTRIBUTE:
            topText->SetLabel("Now Editing Global Attributes");
		    st1->SetLabel("Unused");
		    st2->SetLabel("Unused");
		    st3->SetLabel("Unused");
		    st4->SetLabel("Attributes");
		    for (const auto& pair : Engine::defaultAttributes)
		    {
			    string str = pair.first + ": {default: " + to_string(pair.second) + "}";
			    lb4->Append(str);
		    }
            break;
    }
}
void Editor::resetAction()
{
	lb2->Clear();
	lb3->Clear();
	ActorType* at;
    if (operation == ACTOR)
    {
        at = ActorType::objectMap.at(selObject);
    } 
    else
    {
        at = OverlayType::objectMap.at(selObject);
    }
	auto actionMap = at->actionMap;
	bool found = false;
	for (const auto& pair : actionMap)
	{
		if (pair.first->toString() == selTrigger && !found)
		{
			for (auto const& i : pair.second) {
				lb2->Append(i->toString());
			}
			found = true;
		}
	}
}
void Editor::resetCon()
{
	lb3->Clear();
	ActorType* at;
    if (operation == ACTOR)
    {
        at = ActorType::objectMap.at(selObject);
    } 
    else
    {
        at = OverlayType::objectMap.at(selObject);
    }
	auto actionMap = at->actionMap;
	bool found = false;
	for (const auto& pair : actionMap)
	{
		if (pair.first->toString() == selTrigger && !found)
		{
			for (auto const& i : pair.second) 
			{
				if (i->toString() == selAction && !found)
				{
					for (auto const& j : i->conditionals)
					{
						lb3->Append(j->toString());
					}
					found = true;
				}
			}
		}
	}
}

void Editor::onNew1(wxCommandEvent& event)
{
	if (operation == SPRITE)
	{
		wxFileDialog *openSprDialog = new wxFileDialog(this, "Select Image File");
		openSprDialog->SetWildcard("BMP files (*.bmp)|*.bmp|JPG files (*.jpg)|*.jpg|PNG files (*.png)|*.png");
		wxString fileName;
		if (openSprDialog->ShowModal() == wxID_OK) {
			fileName = openSprDialog->GetPath();
			wxCopyFile(fileName, currentPath + "\\sprites\\" + fileName.AfterLast('\\'));
			Sprite* spr = Sprite::objectMap.at(selObject);
			wxString relPath = wxString(currentPath);
			relPath = relPath.AfterLast('\\');
			relPath += "\\sprites\\" + fileName.AfterLast('\\');
			spr->textrFiles.push_back(relPath.ToStdString());
			lb1->Append(fileName.AfterLast('\\'));
		}
	}
	else if (operation == ACTOR || operation == OVERLAY)
	{
		wxArrayString triggers;
		triggers.Add("Collision");
		triggers.Add("ButtonInput");
		triggers.Add("MouseInput");
		triggers.Add("Create");
		triggers.Add("Step");
		triggers.Add("Draw");
		triggers.Add("Destroy");
		triggers.Add("Timer");
		triggers.Add("Custom");
		wxSingleChoiceDialog *createTriggerDialog = new wxSingleChoiceDialog(this, "Select Trigger to create", "Select Trigger", triggers);
		if (createTriggerDialog->ShowModal() == wxID_OK)
		{
	        ActorType* at;
            if (operation == ACTOR)
            {
                at = ActorType::objectMap.at(selObject);
            } 
            else
            {
                at = OverlayType::objectMap.at(selObject);
            }
			map<string, string> types;
			types.emplace("Collision", "ActorTypeWrapper");
			types.emplace("ButtonInput", "ButtonInputType");
			types.emplace("MouseInput", "MouseInputType");
			types.emplace("Create", "ActorTypeWrapper");
			types.emplace("Step", "ActorTypeWrapper");
			types.emplace("Draw", "ActorTypeWrapper");
			types.emplace("Destroy", "ActorTypeWrapper");
			types.emplace("Timer", "Index");
			types.emplace("Custom", "Index");
			string trig = createTriggerDialog->GetStringSelection().ToStdString();
			string type = types[trig];
			if (type == "ActorTypeWrapper")
			{
				if (trig == "Collision")
				{
					wxArrayString actorChoices;
                    if (operation == ACTOR)
                    {
					    for (const auto& pair : ActorType::objectMap)
					    {
						    actorChoices.Add(pair.first);
					    }
                    } 
                    else
                    {
					    for (const auto& pair : OverlayType::objectMap)
					    {
						    actorChoices.Add(pair.first);
					    }
                    }
					wxSingleChoiceDialog *typeWrapperDialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
					if (typeWrapperDialog->ShowModal() == wxID_OK)
					{
						ActorTypeWrapper* aType;
                        if (operation == ACTOR)
                        {
					        aType = new ActorTypeWrapper(ActorType::objectMap[typeWrapperDialog->GetStringSelection().ToStdString()]);
                        } 
                        else
                        {
					        aType = new ActorTypeWrapper(OverlayType::objectMap[typeWrapperDialog->GetStringSelection().ToStdString()]);
                        } 
                        Trigger* t;
						if (trig == "Collision")
						{
							t = new trigger_preset::Collision(aType);
						}
						list<Action*> aList;
						at->actionMap[t] = aList;
						lb1->Append(t->toString());
					}
				}
				else
				{
					ActorTypeWrapper* aType;
                    if (operation == ACTOR)
                    {
					    aType = new ActorTypeWrapper(ActorType::objectMap[selObject]);
                    } 
                    else
                    {
					    aType = new ActorTypeWrapper(OverlayType::objectMap[selObject]);
                    } 
					Trigger* t;
					if (trig == "Create")
					{
						t = new trigger_preset::Create(aType);
					}
					else if (trig == "Step")
					{
						t = new trigger_preset::Step(aType);
					}
					else if (trig == "Draw")
					{
						t = new trigger_preset::Draw(aType);
					}
					else if (trig == "Destroy")
					{
						t = new trigger_preset::Destroy(aType);
					}
					list<Action*> aList;
					at->actionMap[t] = aList;
					lb1->Append(t->toString());
				}
			}
			else if (type == "ButtonInputType")
			{
				wxArrayString buttonChoices;
				for (const auto& pair : keyNames) {
					if (pair.first >= 0 && pair.first <= 100)
					{
						buttonChoices.Add(pair.second);
					}
				}
				wxArrayString buttonStateChoices;
				buttonStateChoices.Add("Press");
				buttonStateChoices.Add("Release");
				buttonStateChoices.Add("Hold");
				wxSingleChoiceDialog *buttonDialog = new wxSingleChoiceDialog(this, "Choose Button", "Choose one from the list", buttonChoices);
				wxSingleChoiceDialog *buttonStateDialog = new wxSingleChoiceDialog(this, "Choose Button State", "Choose one from the list", buttonStateChoices);
				if (buttonDialog->ShowModal() == wxID_OK)
				{
					string buttonName = buttonDialog->GetStringSelection().ToStdString();
					if (buttonStateDialog->ShowModal() == wxID_OK)
					{
						string buttonState = buttonStateDialog->GetStringSelection().ToStdString();
						ButtonState::ButtonState bState;
						if (buttonState == "Press")
						{
							bState = ButtonState::PRESS;
						}
						else if (buttonState == "Release")
						{
							bState = ButtonState::RELEASE;
						}
						else if (buttonState == "Hold")
						{
							bState = ButtonState::HOLD;
						}
						ButtonInputType* bit = new ButtonInputType();
						bit->state = bState;
						bool find = false;
						for (const auto& pair : keyNames) {
							if (pair.second == buttonName && !find)
							{
								bit->id = pair.first;
								find = true;
							}
							if (find)
							{
								break;
							}
						}
						Trigger *t = new trigger_preset::ButtonInput(bit);
						list<Action*> aList;
						at->actionMap[t] = aList;
						lb1->Append(t->toString());
					}
				}
			}
			else if (type == "MouseInputType")
			{
				wxArrayString mouseChoices;
				mouseChoices.Add("Press");
				mouseChoices.Add("Release");
				mouseChoices.Add("Press On Actor");
				mouseChoices.Add("Release On Actor");
				wxSingleChoiceDialog *mouseDialog = new wxSingleChoiceDialog(this, "Choose Mouse State", "Choose one from the list", mouseChoices);
				if (mouseDialog->ShowModal() == wxID_OK) 
				{
					string mouseState = mouseDialog->GetStringSelection().ToStdString();
					MouseState::MouseState mState;
					if (mouseState == "Press")
					{
						mState = MouseState::PRESS;
					}
					else if (mouseState == "Release")
					{
						mState = MouseState::RELEASE;
					}
					else if (mouseState == "Press On Actor")
					{
						mState = MouseState::PRESS_ON;
					}
					else if (mouseState == "Release On Actor")
					{
						mState = MouseState::RELEASE_ON;
					}
					MouseInputType* mit = new MouseInputType();
					mit->state = mState;
					Trigger* t = new trigger_preset::MouseInput(mit);
					list<Action*> aList;
					at->actionMap[t] = aList;
					lb1->Append(t->toString());
				}
			}
			else if (type == "Index")
			{
				wxTextEntryDialog *indexDialog = new wxTextEntryDialog(this, "Enter Index");
				if (indexDialog->ShowModal() == wxID_OK)
				{
					wxString str = indexDialog->GetValue();
					int ind = stoi(str.ToStdString());
					Index* i = new Index(ind);
					Trigger *t;
					if (trig == "Timer")
					{
						t = new trigger_preset::Timer(i);
					}
					else if (trig == "Custom")
					{
						t = new trigger_preset::Custom(i);
					}
					list<Action*> aList;
					at->actionMap[t] = aList;
					lb1->Append(t->toString());
				}
			}
		}
	}
}
void Editor::onEdit1(wxCommandEvent& event)
{
	int sel = lb1->GetSelection();
	if (sel != -1)
	{
		wxString str = lb1->GetString(sel);
		if (operation == SPRITE)
		{
			Sprite* spr = Sprite::objectMap.at(selObject);
			vector<string>* files = &spr->textrFiles;
			bool found = false;
			for (int i = 0; i < files->size() && !found; i++)
			{
				if (files->at(i) == str)
				{
					files->erase(files->begin() + i);
					lb1->Delete(sel);
					found = true;
				}
			}
			wxFileDialog *openSprDialog = new wxFileDialog(this, "Select Image File");
			wxString fileName;
			if (openSprDialog->ShowModal() == wxID_OK) {
				fileName = openSprDialog->GetPath();
				wxCopyFile(fileName, currentPath + "\\sprites\\" + fileName.AfterLast('\\'));
				Sprite* spr = Sprite::objectMap.at(selObject);
				wxString relPath = wxString(currentPath);
				relPath = relPath.AfterLast('\\');
				relPath += "\\sprites\\" + fileName.AfterLast('\\');
				spr->textrFiles.push_back(relPath.ToStdString());
				lb1->Append(fileName.AfterLast('\\'));
			}
		}
		else if(operation == ACTOR || operation == OVERLAY)
		{
	        ActorType* at;
            if (operation == ACTOR)
            {
                at = ActorType::objectMap.at(selObject);
            } 
            else
            {
                at = OverlayType::objectMap.at(selObject);
            }
		    std::unordered_map<Trigger*, std::list<Action*>, TriggerHash, TriggerEquals>* actionMap = &at->actionMap;
		    bool found = false;
		    for (const auto& pair : *actionMap)
		    {
				if (pair.first->toString() == str.ToStdString() && !found)
				{
					Trigger* t = pair.first;
					list<Action*> aList = pair.second;
					if (t->getTypeName() == "ButtonInput")
					{
						actionMap->erase(pair.first);
						lb1->Delete(sel);
						found = true;
						wxArrayString buttonChoices;
						for (const auto& pair : keyNames) {
							if (pair.first >= 0 && pair.first <= 100)
							{
								buttonChoices.Add(pair.second);
							}
						}
						wxArrayString buttonStateChoices;
						buttonStateChoices.Add("Press");
						buttonStateChoices.Add("Release");
						buttonStateChoices.Add("Hold");
						wxSingleChoiceDialog *buttonDialog = new wxSingleChoiceDialog(this, "Choose Button", "Choose one from the list", buttonChoices);
						wxSingleChoiceDialog *buttonStateDialog = new wxSingleChoiceDialog(this, "Choose Button State", "Choose one from the list", buttonStateChoices);
						if (buttonDialog->ShowModal() == wxID_OK)
						{
							string buttonName = buttonDialog->GetStringSelection().ToStdString();
							if (buttonStateDialog->ShowModal() == wxID_OK)
							{
								string buttonState = buttonStateDialog->GetStringSelection().ToStdString();
								ButtonState::ButtonState bState;
								if (buttonState == "Press")
								{
									bState = ButtonState::PRESS;
								}
								else if (buttonState == "Release")
								{
									bState = ButtonState::RELEASE;
								}
								else if (buttonState == "Hold")
								{
									bState = ButtonState::HOLD;
								}
								ButtonInputType* bit = new ButtonInputType();
								bit->state = bState;
								bool find = false;
								for (const auto& pair : keyNames) {
									if (pair.second == buttonName && !find)
									{
										bit->id = pair.first;
										find = true;
									}
									if (find)
									{
										break;
									}
								}
								Trigger *newTrig = new trigger_preset::ButtonInput(bit);
								at->actionMap[newTrig] = aList;
								lb1->Append(newTrig->toString());
								lb1->SetStringSelection(newTrig->toString());
							}
						}
					}
					else if (t->getTypeName() == "MouseInput")
					{
						actionMap->erase(pair.first);
						lb1->Delete(sel);
						found = true;
						wxArrayString mouseChoices;
						mouseChoices.Add("Press");
						mouseChoices.Add("Release");
						mouseChoices.Add("Press On Actor");
						mouseChoices.Add("Release On Actor");
						wxSingleChoiceDialog *mouseDialog = new wxSingleChoiceDialog(this, "Choose Mouse State", "Choose one from the list", mouseChoices);
						if (mouseDialog->ShowModal() == wxID_OK)
						{
							string mouseState = mouseDialog->GetStringSelection().ToStdString();
							MouseState::MouseState mState;
							if (mouseState == "Press")
							{
								mState = MouseState::PRESS;
							}
							else if (mouseState == "Release")
							{
								mState = MouseState::RELEASE;
							}
							else if (mouseState == "Press On Actor")
							{
								mState = MouseState::PRESS_ON;
							}
							else if (mouseState == "Release On Actor")
							{
								mState = MouseState::RELEASE_ON;
							}
							MouseInputType* mit = new MouseInputType();
							mit->state = mState;
							Trigger* newTrig = new trigger_preset::MouseInput(mit);
							at->actionMap[newTrig] = aList;
							lb1->Append(newTrig->toString());
							lb1->SetStringSelection(newTrig->toString());
						}
					}
					else if (t->getTypeName() == "Timer" || t->getTypeName() == "Custom")
					{
						wxTextEntryDialog *indexDialog = new wxTextEntryDialog(this, "Enter Index");
						if (indexDialog->ShowModal() == wxID_OK)
						{
							wxString str = indexDialog->GetValue();
							int ind = stoi(str.ToStdString());
							Index* i = new Index(ind);
							Trigger *newTrig;
							if (t->getTypeName() == "Timer")
							{
								newTrig = new trigger_preset::Timer(i);
							}
							else if (t->getTypeName() == "Custom")
							{
								newTrig = new trigger_preset::Custom(i);
							}
							actionMap->erase(pair.first);
							lb1->Delete(sel);
							found = true;
							at->actionMap[newTrig] = aList;
							lb1->Append(newTrig->toString());
							lb1->SetStringSelection(newTrig->toString());
						}
					}
					else if(t->getTypeName() == "Collision")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *typeWrapperDialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (typeWrapperDialog->ShowModal() == wxID_OK)
						{
							ActorTypeWrapper* aType;
                            if (operation == ACTOR)
                            {
                                aType =  new ActorTypeWrapper(ActorType::objectMap[typeWrapperDialog->GetStringSelection().ToStdString()]);
                            } 
                            else
                            {
                                aType =  new ActorTypeWrapper(OverlayType::objectMap[typeWrapperDialog->GetStringSelection().ToStdString()]);
                            }
							Trigger* newTrig;
							if (t->getTypeName() == "Collision")
							{
								newTrig = new trigger_preset::Collision(aType);
							}
							actionMap->erase(pair.first);
							lb1->Delete(sel);
							found = true;
							at->actionMap[newTrig] = aList;
							lb1->Append(newTrig->toString());
							lb1->SetStringSelection(newTrig->toString());
						}
					}
					else 
					{
						found = true;
					}
				}
				if (found)
				{
					break;
				}
			}
		}
	}
}
void Editor::onDelete1(wxCommandEvent& event)
{
	int sel = lb1->GetSelection();
	wxString str = lb1->GetString(sel);
	if (sel != -1)
	{
        switch (operation)
        {
            case SPRITE:
            {
			    Sprite* spr = Sprite::objectMap.at(selObject);
			    vector<string>* files = &spr->textrFiles;
			    bool found = false;
			    for (int i = 0; i < files->size() && !found; i++)
			    {
				    wxString file = files->at(i);
				    if (file.AfterLast('\\') == str)
				    {
					    files->erase(files->begin() + i);
					    lb1->Delete(sel);
					    found = true;
				    }
			    }
                break;
            }
            case ACTOR:
            case OVERLAY:
            {
	            ActorType* at;
                if (operation == ACTOR)
                {
                    at = ActorType::objectMap.at(selObject);
                } 
                else
                {
                    at = OverlayType::objectMap.at(selObject);
                }
			    std::unordered_map<Trigger*, std::list<Action*>, TriggerHash, TriggerEquals>* actionMap = &at->actionMap;
			    bool found = false;
			    for (const auto& pair : *actionMap)
			    {
				    if (pair.first->toString() == str.ToStdString() && !found)
				    {
					    actionMap->erase(pair.first);
					    lb1->Delete(sel);
					    found = true;
				    }
					if (found)
					{
						break;
					}
			    }
                break;
            }
        }
	}
}

void Editor::onNew2(wxCommandEvent& event)
{
	if (lb1->GetSelection() != -1)
	{
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto& actionMap = at->actionMap;
		bool found = false;
		for (auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				Trigger* t = pair.first;
				list<Action*> *aList = &pair.second;
				wxArrayString actionChoices;
				actionChoices.Add("Apply Force");
				actionChoices.Add("Set X Speed");
				actionChoices.Add("Set Y Speed");
				actionChoices.Add("Move To Nearest");
				actionChoices.Add("Move Toward Nearest");
				actionChoices.Add("Move To");
				actionChoices.Add("Move");
				actionChoices.Add("Create");
				actionChoices.Add("Set Attribute");
				actionChoices.Add("Change Attribute");
				actionChoices.Add("Destroy");
				actionChoices.Add("Set Timer");
				actionChoices.Add("Call Custom");
				actionChoices.Add("Play Sound");
				actionChoices.Add("Play Music");
				actionChoices.Add("Stop Music");
				actionChoices.Add("Set Room");
				actionChoices.Add("Change Room");
				actionChoices.Add("Collidable");
				actionChoices.Add("Uses Gravity");
				actionChoices.Add("Set Animation Speed");
				actionChoices.Add("Set Animation Frame");
				actionChoices.Add("Set Depth");
				actionChoices.Add("Set X Scale");
				actionChoices.Add("Set Y Scale");
				actionChoices.Add("Set Friction");
				actionChoices.Add("Set Max X Speed");
				actionChoices.Add("Set Max Y Speed");
				actionChoices.Add("Set Sprite");
				wxSingleChoiceDialog *actionChoiceDialog = new wxSingleChoiceDialog(this, "Choose Action", "Choose one from the list", actionChoices);
				if (actionChoiceDialog->ShowModal() == wxID_OK)
				{
					map<string, string> types;
					types.emplace("Apply Force", "FF");
					types.emplace("Set X Speed", "F");
					types.emplace("Set Y Speed", "F");
					types.emplace("Move To Nearest", "At");
					types.emplace("Move Toward Nearest", "AtF");
					types.emplace("Move To", "FF");
					types.emplace("Move", "FF");
					types.emplace("Create", "AtFF");
					types.emplace("Set Attribute", "StI");
					types.emplace("Change Attribute", "StI");
					types.emplace("Destroy", "NA");
					types.emplace("Set Timer", "IF");
					types.emplace("Call Custom", "I");
					types.emplace("Play Sound", "So");
					types.emplace("Play Music", "M");
					types.emplace("Stop Music", "M");
					types.emplace("Set Room", "St");
					types.emplace("Change Room", "I");
					types.emplace("Collidable", "B");
					types.emplace("Uses Gravity", "B");
					types.emplace("Set Animation Speed", "F");
					types.emplace("Set Animation Frame", "F");
					types.emplace("Set Depth", "F");
					types.emplace("Set X Scale", "F");
					types.emplace("Set Y Scale", "F");
					types.emplace("Set Friction", "F");
					types.emplace("Set Max X Speed", "F");
					types.emplace("Set Max Y Speed", "F");
					types.emplace("Set Sprite", "St");
					string action = actionChoiceDialog->GetStringSelection().ToStdString();
					string aType = types[action];
					if (aType == "FF")
					{
						wxTextEntryDialog *float3ChoiceDialog = new wxTextEntryDialog(this, "Enter X");
						if (float3ChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = float3ChoiceDialog->GetValue();
							float spd1 = stof(str.ToStdString());
							wxTextEntryDialog *float3ChoiceDialog = new wxTextEntryDialog(this, "Enter Y");
							if (float3ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float3ChoiceDialog->GetValue();
								float spd2 = stof(str.ToStdString());
								Action* a;
								if (action == "Apply Force")
								{
									a = new action_preset::ApplyForce(spd1, spd2);
								}
								else if (action == "Move To")
								{
									a = new action_preset::MoveTo(spd1, spd2);
								}
								else if (action == "Move")
								{
									a = new action_preset::Move(spd1, spd2);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "B")
					{
						wxArrayString tf;
						tf.Add("True");
						tf.Add("False");
						wxSingleChoiceDialog *boolChoiceDialog = new wxSingleChoiceDialog(this, "Collidable", "Set True or False", tf);
						if (boolChoiceDialog->ShowModal() == wxID_OK)
						{
							string answer = boolChoiceDialog->GetStringSelection().ToStdString();
							Action *a;
							if (answer == "True")
							{
								if (action == "Collidable")
								{
									a = new action_preset::CollidableSet(true);
								}
								else
								{
									a = new action_preset::GravitousSet(true);
								}
							}
							else
							{
								if (action == "Collidable")
								{
									a = new action_preset::CollidableSet(false);
								}
								else
								{
									a = new action_preset::GravitousSet(false);
								}
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "F")
					{
						wxTextEntryDialog *floatChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
						if (floatChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = floatChoiceDialog->GetValue();
							float spd = stof(str.ToStdString());
							Action* a;
							if (action == "Set X Speed")
							{
								a = new action_preset::SetXSpeed(spd);
							}
							else if (action == "Set Y Speed")
							{
								a = new action_preset::SetYSpeed(spd);
							}
							else if (action == "Set Animation Speed")
							{
								a = new action_preset::AnimationSpeedSet(spd);
							}
							else if (action == "Set Animation Frame")
							{
								a = new action_preset::AnimationFrameSet(spd);
							}
							else if (action == "Set Depth")
							{
								a = new action_preset::DepthSet(spd);
							}
							else if (action == "Set X Scale")
							{
								a = new action_preset::XScaleSet(spd);
							}
							else if (action == "Set Y Scale")
							{
								a = new action_preset::YScaleSet(spd);
							}
							else if (action == "Set Friction")
							{
								a = new action_preset::FrictionSet(spd);
							}
							else if (action == "Set Max X Speed")
							{
								a = new action_preset::MaxXSpeedSet(spd);
							}
							else if (action == "Set Max Y Speed")
							{
								a = new action_preset::MaxYSpeedSet(spd);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "At")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actorDialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actorDialog->ShowModal() == wxID_OK)
						{
							wxString str = actorDialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							Action *a = new action_preset::MoveToNearest(act);
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "AtF")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actor2Dialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actor2Dialog->ShowModal() == wxID_OK)
						{
							wxString str = actor2Dialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							wxTextEntryDialog *float2ChoiceDialog = new wxTextEntryDialog(this, "Enter Speed");
							if (float2ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float2ChoiceDialog->GetValue();
								float spd = stof(str.ToStdString());
								Action *a = new action_preset::MoveTowardNearest(act, spd);
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "AtFF")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actor3Dialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actor3Dialog->ShowModal() == wxID_OK)
						{
							wxString str = actor3Dialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							wxTextEntryDialog *float5ChoiceDialog = new wxTextEntryDialog(this, "Enter X");
							if (float5ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float5ChoiceDialog->GetValue();
								float spd1 = stof(str.ToStdString());
								wxTextEntryDialog *float6ChoiceDialog = new wxTextEntryDialog(this, "Enter Y");
								if (float6ChoiceDialog->ShowModal() == wxID_OK)
								{
									wxString str = float6ChoiceDialog->GetValue();
									float spd2 = stof(str.ToStdString());
									State st = State(spd1, spd2);
									Action *a = new action_preset::Create(act, st);
									aList->emplace_back(a);
									lb2->Append(a->toString());
									lb2->SetStringSelection(a->toString());
								}
							}
						}
					}
					else if (aType == "NA")
					{
						Action* a = new action_preset::Destroy();
						aList->emplace_back(a);
						lb2->Append(a->toString());
						lb2->SetStringSelection(a->toString());
					}
					else if (aType == "StI")
					{
						wxArrayString attrChoices;
						std::unordered_map<std::string, int> attributes = at->attributes;
						for (const auto& pair : attributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						for (const auto& pair : Engine::defaultAttributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						wxSingleChoiceDialog *attrChoiceDialog = new wxSingleChoiceDialog(this, "Choose Attribute", "Choose one from the list", attrChoices);
						if (attrChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = attrChoiceDialog->GetStringSelection();
							string nm = str.ToStdString();
							wxTextEntryDialog *int3ChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
							if (int3ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = int3ChoiceDialog->GetValue();
								int ind = stoi(str.ToStdString());
								Action *a;
								if (action == "Set Attribute")
								{
									a = new action_preset::AttributeSet(nm, ind);
								}
								else if (action == "Change Attribute")
								{
									a = new action_preset::AttributeChange(nm, ind);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "IF")
					{
					wxTextEntryDialog *int2ChoiceDialog = new wxTextEntryDialog(this, "Enter Index");
					if (int2ChoiceDialog->ShowModal() == wxID_OK)
					{
						wxString str = int2ChoiceDialog->GetValue();
						int ind = stoi(str.ToStdString());
						wxTextEntryDialog *float4ChoiceDialog = new wxTextEntryDialog(this, "Enter Time");
						if (float4ChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = float4ChoiceDialog->GetValue();
							float spd = stof(str.ToStdString());
							Action* a = new action_preset::SetTimer(ind, spd);
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					}
					else if (aType == "I")
					{
						wxTextEntryDialog *intChoiceDialog = new wxTextEntryDialog(this, "Enter Index");
						if (intChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = intChoiceDialog->GetValue();
							int ind = stoi(str.ToStdString());
							Action* a;
							if (action == "Call Custom")
							{
								a = new action_preset::CallCustom(ind);
							}
							else if (action == "Change Room")
							{
								a = new action_preset::ChangeRoom(ind);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "St")
					{
						if (action == "Set Room")
						{
							wxArrayString roomChoices;
							for (const auto& pair : Room::objectMap)
							{
								roomChoices.Add(pair.first);
							}
							wxSingleChoiceDialog *roomChoiceDialog = new wxSingleChoiceDialog(this, "Choose Room", "Choose one from the list", roomChoices);
							if (roomChoiceDialog->ShowModal() == wxID_OK)
							{
								string str = roomChoiceDialog->GetStringSelection().ToStdString();
								Action* a;
								if (action == "Set Room")
								{
									a = new action_preset::SetRoom(str);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
						else
						{
							wxArrayString sprChoices;
							for (const auto& pair : Sprite::objectMap)
							{
								sprChoices.Add(pair.first);
							}
							wxSingleChoiceDialog *sprChoiceDialog = new wxSingleChoiceDialog(this, "Choose Sprite", "Choose one from the list", sprChoices);
							if (sprChoiceDialog->ShowModal() == wxID_OK)
							{
								string str = sprChoiceDialog->GetStringSelection().ToStdString();
								Action* a;
								if (action == "Set Sprite")
								{
									a = new action_preset::SetRoom(str);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "So")
					{
						wxArrayString soundChoices;
						for (const auto& pair : Sound::objectMap)
						{
							soundChoices.Add(pair.first);
						}
						wxSingleChoiceDialog *soundChoiceDialog = new wxSingleChoiceDialog(this, "Choose Sound", "Choose one from the list", soundChoices);
						if (soundChoiceDialog->ShowModal() == wxID_OK)
						{
							Sound* s = Sound::objectMap[soundChoiceDialog->GetStringSelection().ToStdString()];
							Action* a = new action_preset::PlaySound(s);
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "M")
					{
						wxArrayString musicChoices;
						for (const auto& pair : Music::objectMap)
						{
							musicChoices.Add(pair.first);
						}
						wxSingleChoiceDialog *musicChoiceDialog = new wxSingleChoiceDialog(this, "Choose Music", "Choose one from the list", musicChoices);
						if (musicChoiceDialog->ShowModal() == wxID_OK)
						{
							Music* m = Music::objectMap[musicChoiceDialog->GetStringSelection().ToStdString()];
							Action* a;
							if (action == "Play Music")
							{
								a = new action_preset::PlayMusic(m);
							}
							else if (action == "Stop Music")
							{
								a = new action_preset::StopMusic(m);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					found = true;
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}
void Editor::onEdit2(wxCommandEvent& event)
{
	int sel = lb2->GetSelection();
	if (sel != -1)
	{
		wxString str = lb2->GetString(sel);
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto& actionMap = at->actionMap;
		bool found = false;
		for (auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				for (std::list<Action*>::iterator it = pair.second.begin(); it != pair.second.end() && !found; ++it) {
					Action* a = *it;
					if (a->toString() == str.ToStdString())
					{
						pair.second.erase(it);
						found = true;
					}
				}
			}
			if (found)
			{
				break;
			}
		}
	}
	if (lb1->GetSelection() != -1)
	{
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto& actionMap = at->actionMap;
		bool found = false;
		for (auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				Trigger* t = pair.first;
				list<Action*> *aList = &pair.second;
				wxArrayString actionChoices;
				actionChoices.Add("Apply Force");
				actionChoices.Add("Set X Speed");
				actionChoices.Add("Set Y Speed");
				actionChoices.Add("Move To Nearest");
				actionChoices.Add("Move Toward Nearest");
				actionChoices.Add("Move To");
				actionChoices.Add("Move");
				actionChoices.Add("Create");
				actionChoices.Add("Set Attribute");
				actionChoices.Add("Change Attribute");
				actionChoices.Add("Destroy");
				actionChoices.Add("Set Timer");
				actionChoices.Add("Call Custom");
				actionChoices.Add("Play Sound");
				actionChoices.Add("Play Music");
				actionChoices.Add("Stop Music");
				actionChoices.Add("Set Room");
				actionChoices.Add("Change Room");
				actionChoices.Add("Collidable");
				actionChoices.Add("Uses Gravity");
				actionChoices.Add("Set Animation Speed");
				actionChoices.Add("Set Animation Frame");
				actionChoices.Add("Set Depth");
				actionChoices.Add("Set X Scale");
				actionChoices.Add("Set Y Scale");
				actionChoices.Add("Set Friction");
				actionChoices.Add("Set Max X Speed");
				actionChoices.Add("Set Max Y Speed");
				actionChoices.Add("Set Sprite");
				wxSingleChoiceDialog *actionChoiceDialog = new wxSingleChoiceDialog(this, "Choose Action", "Choose one from the list", actionChoices);
				if (actionChoiceDialog->ShowModal() == wxID_OK)
				{
					map<string, string> types;
					types.emplace("Apply Force", "FF");
					types.emplace("Set X Speed", "F");
					types.emplace("Set Y Speed", "F");
					types.emplace("Move To Nearest", "At");
					types.emplace("Move Toward Nearest", "AtF");
					types.emplace("Move To", "FF");
					types.emplace("Move", "FF");
					types.emplace("Create", "AtFF");
					types.emplace("Set Attribute", "StI");
					types.emplace("Change Attribute", "StI");
					types.emplace("Destroy", "NA");
					types.emplace("Set Timer", "IF");
					types.emplace("Call Custom", "I");
					types.emplace("Play Sound", "So");
					types.emplace("Play Music", "M");
					types.emplace("Stop Music", "M");
					types.emplace("Set Room", "St");
					types.emplace("Change Room", "I");
					types.emplace("Collidable", "B");
					types.emplace("Uses Gravity", "B");
					types.emplace("Set Animation Speed", "F");
					types.emplace("Set Animation Frame", "F");
					types.emplace("Set Depth", "F");
					types.emplace("Set X Scale", "F");
					types.emplace("Set Y Scale", "F");
					types.emplace("Set Friction", "F");
					types.emplace("Set Max X Speed", "F");
					types.emplace("Set Max Y Speed", "F");
					types.emplace("Set Sprite", "St");
					string action = actionChoiceDialog->GetStringSelection().ToStdString();
					string aType = types[action];
					if (aType == "FF")
					{
						wxTextEntryDialog *float3ChoiceDialog = new wxTextEntryDialog(this, "Enter X");
						if (float3ChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = float3ChoiceDialog->GetValue();
							float spd1 = stof(str.ToStdString());
							wxTextEntryDialog *float3ChoiceDialog = new wxTextEntryDialog(this, "Enter Y");
							if (float3ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float3ChoiceDialog->GetValue();
								float spd2 = stof(str.ToStdString());
								Action* a;
								if (action == "Apply Force")
								{
									a = new action_preset::ApplyForce(spd1, spd2);
								}
								else if (action == "Move To")
								{
									a = new action_preset::MoveTo(spd1, spd2);
								}
								else if (action == "Move")
								{
									a = new action_preset::Move(spd1, spd2);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "B")
					{
						wxArrayString tf;
						tf.Add("True");
						tf.Add("False");
						wxSingleChoiceDialog *boolChoiceDialog = new wxSingleChoiceDialog(this, "Collidable", "Set True or False", tf);
						if (boolChoiceDialog->ShowModal() == wxID_OK)
						{
							string answer = boolChoiceDialog->GetStringSelection().ToStdString();
							Action *a;
							if (answer == "True")
							{
								if (action == "Collidable")
								{
									a = new action_preset::CollidableSet(true);
								}
								else
								{
									a = new action_preset::GravitousSet(true);
								}
							}
							else
							{
								if (action == "Collidable")
								{
									a = new action_preset::CollidableSet(false);
								}
								else
								{
									a = new action_preset::GravitousSet(false);
								}
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "F")
					{
						wxTextEntryDialog *floatChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
						if (floatChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = floatChoiceDialog->GetValue();
							float spd = stof(str.ToStdString());
							Action* a;
							if (action == "Set X Speed")
							{
								a = new action_preset::SetXSpeed(spd);
							}
							else if (action == "Set Y Speed")
							{
								a = new action_preset::SetYSpeed(spd);
							}
							else if (action == "Set Animation Speed")
							{
								a = new action_preset::AnimationSpeedSet(spd);
							}
							else if (action == "Set Animation Frame")
							{
								a = new action_preset::AnimationFrameSet(spd);
							}
							else if (action == "Set Depth")
							{
								a = new action_preset::DepthSet(spd);
							}
							else if (action == "Set X Scale")
							{
								a = new action_preset::XScaleSet(spd);
							}
							else if (action == "Set Y Scale")
							{
								a = new action_preset::YScaleSet(spd);
							}
							else if (action == "Set Friction")
							{
								a = new action_preset::FrictionSet(spd);
							}
							else if (action == "Set Max X Speed")
							{
								a = new action_preset::MaxXSpeedSet(spd);
							}
							else if (action == "Set Max Y Speed")
							{
								a = new action_preset::MaxYSpeedSet(spd);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "At")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actorDialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actorDialog->ShowModal() == wxID_OK)
						{
							wxString str = actorDialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							Action *a = new action_preset::MoveToNearest(act);
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "AtF")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actor2Dialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actor2Dialog->ShowModal() == wxID_OK)
						{
							wxString str = actor2Dialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							wxTextEntryDialog *float2ChoiceDialog = new wxTextEntryDialog(this, "Enter Speed");
							if (float2ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float2ChoiceDialog->GetValue();
								float spd = stof(str.ToStdString());
								Action *a = new action_preset::MoveTowardNearest(act, spd);
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "AtFF")
					{
					    wxArrayString actorChoices;
                        if (operation == ACTOR)
                        {
					        for (const auto& pair : ActorType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        } 
                        else
                        {
					        for (const auto& pair : OverlayType::objectMap)
					        {
						        actorChoices.Add(pair.first);
					        }
                        }
						wxSingleChoiceDialog *actor3Dialog = new wxSingleChoiceDialog(this, "Choose Actor Type", "Choose one from the list", actorChoices);
						if (actor3Dialog->ShowModal() == wxID_OK)
						{
							wxString str = actor3Dialog->GetStringSelection();
	                        ActorType* act;
                            if (operation == ACTOR)
                            {
                                act = ActorType::objectMap[str.ToStdString()];
                            } 
                            else
                            {
                                act = OverlayType::objectMap[str.ToStdString()];
                            }
							wxTextEntryDialog *float5ChoiceDialog = new wxTextEntryDialog(this, "Enter X");
							if (float5ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float5ChoiceDialog->GetValue();
								float spd1 = stof(str.ToStdString());
								wxTextEntryDialog *float6ChoiceDialog = new wxTextEntryDialog(this, "Enter Y");
								if (float6ChoiceDialog->ShowModal() == wxID_OK)
								{
									wxString str = float6ChoiceDialog->GetValue();
									float spd2 = stof(str.ToStdString());
									State st = State(spd1, spd2);
									Action *a = new action_preset::Create(act, st);
									aList->emplace_back(a);
									lb2->Append(a->toString());
									lb2->SetStringSelection(a->toString());
								}
							}
						}
					}
					else if (aType == "NA")
					{
						Action* a = new action_preset::Destroy();
						aList->emplace_back(a);
						lb2->Append(a->toString());
						lb2->SetStringSelection(a->toString());
					}
					else if (aType == "StI")
					{
						wxArrayString attrChoices;
						std::unordered_map<std::string, int> attributes = at->attributes;
						for (const auto& pair : attributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						for (const auto& pair : Engine::defaultAttributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						wxSingleChoiceDialog *attrChoiceDialog = new wxSingleChoiceDialog(this, "Choose Attribute", "Choose one from the list", attrChoices);
						if (attrChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = attrChoiceDialog->GetStringSelection();
							string nm = str.ToStdString();
							wxTextEntryDialog *int3ChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
							if (int3ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = int3ChoiceDialog->GetValue();
								int ind = stoi(str.ToStdString());
								Action *a;
								if (action == "Set Attribute")
								{
									a = new action_preset::AttributeSet(nm, ind);
								}
								else if (action == "Change Attribute")
								{
									a = new action_preset::AttributeChange(nm, ind);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "IF")
					{
						wxTextEntryDialog *int2ChoiceDialog = new wxTextEntryDialog(this, "Enter Index");
						if (int2ChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = int2ChoiceDialog->GetValue();
							int ind = stoi(str.ToStdString());
							wxTextEntryDialog *float4ChoiceDialog = new wxTextEntryDialog(this, "Enter Time");
							if (float4ChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = float4ChoiceDialog->GetValue();
								float spd = stof(str.ToStdString());
								Action* a = new action_preset::SetTimer(ind, spd);
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "I")
					{
						wxTextEntryDialog *intChoiceDialog = new wxTextEntryDialog(this, "Enter Index");
						if (intChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = intChoiceDialog->GetValue();
							int ind = stoi(str.ToStdString());
							Action* a;
							if (action == "Call Custom")
							{
								a = new action_preset::CallCustom(ind);
							}
							else if (action == "Change Room")
							{
								a = new action_preset::ChangeRoom(ind);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "St")
					{
						if (action == "Set Room")
						{
							wxArrayString roomChoices;
							for (const auto& pair : Room::objectMap)
							{
								roomChoices.Add(pair.first);
							}
							wxSingleChoiceDialog *roomChoiceDialog = new wxSingleChoiceDialog(this, "Choose Room", "Choose one from the list", roomChoices);
							if (roomChoiceDialog->ShowModal() == wxID_OK)
							{
								string str = roomChoiceDialog->GetStringSelection().ToStdString();
								Action* a;
								if (action == "Set Room")
								{
									a = new action_preset::SetRoom(str);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
						else
						{
							wxArrayString sprChoices;
							for (const auto& pair : Sprite::objectMap)
							{
								sprChoices.Add(pair.first);
							}
							wxSingleChoiceDialog *sprChoiceDialog = new wxSingleChoiceDialog(this, "Choose Sprite", "Choose one from the list", sprChoices);
							if (sprChoiceDialog->ShowModal() == wxID_OK)
							{
								string str = sprChoiceDialog->GetStringSelection().ToStdString();
								Action* a;
								if (action == "Set Sprite")
								{
									a = new action_preset::SpriteSet(str);
								}
								aList->emplace_back(a);
								lb2->Append(a->toString());
								lb2->SetStringSelection(a->toString());
							}
						}
					}
					else if (aType == "So")
					{
						wxArrayString soundChoices;
						for (const auto& pair : Sound::objectMap)
						{
							soundChoices.Add(pair.first);
						}
						wxSingleChoiceDialog *soundChoiceDialog = new wxSingleChoiceDialog(this, "Choose Sound", "Choose one from the list", soundChoices);
						if (soundChoiceDialog->ShowModal() == wxID_OK)
						{
							Sound* s = Sound::objectMap[soundChoiceDialog->GetStringSelection().ToStdString()];
							Action* a = new action_preset::PlaySound(s);
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					else if (aType == "M")
					{
						wxArrayString musicChoices;
						for (const auto& pair : Music::objectMap)
						{
							musicChoices.Add(pair.first);
						}
						wxSingleChoiceDialog *musicChoiceDialog = new wxSingleChoiceDialog(this, "Choose Music", "Choose one from the list", musicChoices);
						if (musicChoiceDialog->ShowModal() == wxID_OK)
						{
							Music* m = Music::objectMap[musicChoiceDialog->GetStringSelection().ToStdString()];
							Action* a;
							if (action == "Play Music")
							{
								a = new action_preset::PlayMusic(m);
							}
							else if (action == "Stop Music")
							{
								a = new action_preset::StopMusic(m);
							}
							aList->emplace_back(a);
							lb2->Append(a->toString());
							lb2->SetStringSelection(a->toString());
						}
					}
					found = true;
					lb2->Delete(sel);
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}
void Editor::onDelete2(wxCommandEvent& event)
{
	int sel = lb2->GetSelection();
	if (sel != -1)
	{
		wxString str = lb2->GetString(sel);
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto& actionMap = at->actionMap;
		bool found = false;
		for (auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				for (std::list<Action*>::iterator it = pair.second.begin(); it != pair.second.end() && !found; ++it) {
					Action* a = *it;
					if (a->toString() == str.ToStdString())
					{
						pair.second.erase(it);
						lb2->Delete(sel);
						found = true;
					}
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}

void Editor::onNew3(wxCommandEvent& event)
{
	int sel = lb2->GetSelection();
	if (sel != -1)
	{
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto actionMap = at->actionMap;
		bool found = false;
		for (const auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				for (auto const& i : pair.second)
				{
					if (i->toString() == selAction && !found)
					{
						list<Conditional*> *cList = &i->conditionals;
						wxArrayString attrChoices;
						std::unordered_map<std::string, int> attributes = at->attributes;
						for (const auto& pair : attributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						for (const auto& pair : Engine::defaultAttributes)
						{
							string str = pair.first;
							attrChoices.Add(str);
						}
						wxSingleChoiceDialog *attr2ChoiceDialog = new wxSingleChoiceDialog(this, "Choose Attribute", "Choose one from the list", attrChoices);
						if (attr2ChoiceDialog->ShowModal() == wxID_OK)
						{
							wxString str = attr2ChoiceDialog->GetStringSelection();
							string key = str.ToStdString();
							map<string, Comparison> types;
							types.emplace("==", EQUAL);
							types.emplace("=/=", NOT_EQUAL);
							types.emplace("<", LESS_THAN);
							types.emplace("<=", LESS_THAN_EQUAL);
							types.emplace(">", GREATER_THAN);
							types.emplace(">=", GREATER_THAN_EQUAL);
							wxArrayString compChoices;
							compChoices.Add("==");
							compChoices.Add("=/=");
							compChoices.Add("<");
							compChoices.Add("<=");
							compChoices.Add(">");
							compChoices.Add(">=");
							wxSingleChoiceDialog *compChoiceDialog = new wxSingleChoiceDialog(this, "Choose Comparison", "Choose one from the list", compChoices);
							if (compChoiceDialog->ShowModal() == wxID_OK)
							{
								wxString str = compChoiceDialog->GetStringSelection();
								Comparison c = types[str.ToStdString()];
								wxTextEntryDialog *intChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
								if (intChoiceDialog->ShowModal() == wxID_OK)
								{
									wxString str = intChoiceDialog->GetValue();
									int val = stoi(str.ToStdString());
									Conditional* cnd = new Conditional(c, key, val);
									cList->emplace_back(cnd);
									lb3->Append(cnd->toString());
									lb3->SetStringSelection(cnd->toString());
								}
							}
						}
						found = true;
					}
				}
				if (found)
				{
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}
void Editor::onEdit3(wxCommandEvent& event)
{
	int sel = lb3->GetSelection();
	if (sel != -1)
	{
		wxString str = lb3->GetString(sel);
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto actionMap = at->actionMap;
		bool found = false;
		for (const auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				for (auto const& i : pair.second)
				{
					if (i->toString() == selAction && !found)
					{
						for (std::list<Conditional*>::iterator it = i->conditionals.begin(); it != i->conditionals.end() && !found; ++it) {
							Conditional* cnd = *it;
							if (cnd->toString() == str.ToStdString())
							{
								found = true;
								map<string, Comparison> types;
								types.emplace("==", EQUAL);
								types.emplace("=/=", NOT_EQUAL);
								types.emplace("<", LESS_THAN);
								types.emplace("<=", LESS_THAN_EQUAL);
								types.emplace(">", GREATER_THAN);
								types.emplace(">=", GREATER_THAN_EQUAL);
								wxArrayString compChoices;
								compChoices.Add("==");
								compChoices.Add("=/=");
								compChoices.Add("<");
								compChoices.Add("<=");
								compChoices.Add(">");
								compChoices.Add(">=");
								wxSingleChoiceDialog *compChoiceDialog = new wxSingleChoiceDialog(this, "Choose Comparison", "Choose one from the list", compChoices);
								if (compChoiceDialog->ShowModal() == wxID_OK)
								{
									wxString str = compChoiceDialog->GetStringSelection();
									Comparison c = types[str.ToStdString()];
									wxTextEntryDialog *intChoiceDialog = new wxTextEntryDialog(this, "Enter Value");
									if (intChoiceDialog->ShowModal() == wxID_OK)
									{
										wxString str = intChoiceDialog->GetValue();
										int val = stoi(str.ToStdString());
										cnd->comparison = c;
										cnd->value = val;
										lb3->Delete(sel);
										lb3->Append(cnd->toString());
										lb3->SetStringSelection(cnd->toString());
									}
								}
							}
						}
					}
				}
				if (found)
				{
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}
void Editor::onDelete3(wxCommandEvent& event)
{
	int sel = lb3->GetSelection();
	if (sel != -1)
	{
		wxString str = lb3->GetString(sel);
	    ActorType* at;
        if (operation == ACTOR)
        {
            at = ActorType::objectMap.at(selObject);
        } 
        else
        {
            at = OverlayType::objectMap.at(selObject);
        }
		auto actionMap = at->actionMap;
		bool found = false;
		for (const auto& pair : actionMap)
		{
			if (pair.first->toString() == selTrigger && !found)
			{
				for (auto const& i : pair.second)
				{
					if (i->toString() == selAction && !found)
					{
						for (std::list<Conditional*>::iterator it = i->conditionals.begin(); it != i->conditionals.end() && !found; ++it) {
							Conditional* cnd = *it;
							if (cnd->toString() == str.ToStdString())
							{
								i->conditionals.erase(it);
								lb3->Delete(sel);
								found = true;
							}
						}
					}
				}
				if (found)
				{
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
	}
}

void Editor::onNew4(wxCommandEvent& event)
{
	wxString str;
	wxTextEntryDialog* getNewAttr = new wxTextEntryDialog(this, "Enter New Attribute Name");
	if (selObject != "")
	{
		if (getNewAttr->ShowModal() == wxID_OK)
		{
			str = getNewAttr->GetValue();
	        ActorType* at;
            if (operation == ACTOR)
            {
                at = ActorType::objectMap.at(selObject);
            } 
            else
            {
                at = OverlayType::objectMap.at(selObject);
            }
			std::unordered_map<std::string, int>* attr = &at->attributes;
			attr->emplace(str.ToStdString(), 0);
			string ret = str.ToStdString() + ": {default: 0}";
			lb4->Append(ret);
		}
	}
    else if (operation == ATTRIBUTE)
    {
		if (getNewAttr->ShowModal() == wxID_OK)
		{
			str = getNewAttr->GetValue();
			Engine::defaultAttributes[str.ToStdString()] = 0;
			string ret = str.ToStdString() + ": {default: 0}";
			lb4->Append(ret);
		}
    }
}
void Editor::onEdit4(wxCommandEvent& event)
{
	wxString str;
	long toEdit;
	wxArrayString sprChoices;
	for (const auto& pair : Sprite::objectMap)
	{
		sprChoices.Add(pair.first);
	}
	sprChoices.Add("None");
	wxSingleChoiceDialog* editSpr = new wxSingleChoiceDialog(this, "Edit Sprite", "Choose one from the list", sprChoices);
	wxTextEntryDialog* editAttr = new wxTextEntryDialog(this, "Edit Attribute Value");
	int sel = lb4->GetSelection();
	if (sel != -1)
	{
        switch (operation)
        {
            case ACTOR:
            case OVERLAY:
				if (lb4->GetString(sel).BeforeFirst(':') != "Sprite")
				{
					if (editAttr->ShowModal() == wxID_OK)
					{
						str = editAttr->GetValue();
						str.ToLong(&toEdit);
						str = lb4->GetString(sel);
	                    ActorType* at;
                        if (operation == ACTOR)
                        {
                            at = ActorType::objectMap.at(selObject);
                        } 
                        else
                        {
                            at = OverlayType::objectMap.at(selObject);
                        }
						std::unordered_map<std::string, int>* attr = &at->attributes;
						attr->erase((str.BeforeFirst(':')).ToStdString());
						attr->emplace((str.BeforeFirst(':')).ToStdString(), (int)toEdit);
						string ret = (str.BeforeFirst(':')).ToStdString() + ": {default: " + to_string(toEdit) + "}";
						lb4->SetString(sel, ret);
					}
				}
				else
				{
					if (editSpr->ShowModal() == wxID_OK)
					{
						wxString str = editSpr->GetStringSelection();
	                    ActorType* at;
                        if (operation == ACTOR)
                        {
                            at = ActorType::objectMap.at(selObject);
                        } 
                        else
                        {
                            at = OverlayType::objectMap.at(selObject);
                        }
						if (str == "None")
						{
							at->sprite = NULL;
							lb4->SetString(sel, "Sprite: {None}");
						}
						else
						{
							Sprite* spr = Sprite::objectMap[str.ToStdString()];
							at->sprite = spr;
							lb4->SetString(sel, "Sprite: {" + at->sprite->name + "}");
						}
					}
				}
                break;
            case ATTRIBUTE:
		        if (editAttr->ShowModal() == wxID_OK)
		        {
			        str = editAttr->GetValue();
			        str.ToLong(&toEdit);
			        str = lb4->GetString(sel);
			        Engine::defaultAttributes[(str.BeforeFirst(':')).ToStdString()] = (int)toEdit;
			        string ret = (str.BeforeFirst(':')).ToStdString() + ": {default: " + to_string(toEdit) + "}";
			        lb4->SetString(sel, ret);
		        }
                break;
        }
	}
}
void Editor::onDelete4(wxCommandEvent& event)
{
	int sel = lb4->GetSelection();
	if (sel != -1)
	{
		wxString str = lb4->GetString(sel);
        switch (operation)
        {
            case ACTOR:
            case OVERLAY:
            {
				if (str.BeforeFirst(':') != "Sprite")
				{
	                ActorType* at;
                    if (operation == ACTOR)
                    {
                        at = ActorType::objectMap.at(selObject);
                    } 
                    else
                    {
                        at = OverlayType::objectMap.at(selObject);
                    }
					std::unordered_map<std::string, int>* attr = &at->attributes;
					attr->erase((str.BeforeFirst(':')).ToStdString());
					lb4->Delete(sel);
				}
                break;
            }
            case ATTRIBUTE:
		        Engine::defaultAttributes.erase((str.BeforeFirst(':')).ToStdString());
		        lb4->Delete(sel);
                break;
        }
	}
}

void Editor::onBox1Select(wxCommandEvent& event)
{
	int sel = lb1->GetSelection();
	if (sel != -1 && (operation == ACTOR || operation == OVERLAY))
	{
		wxString str = lb1->GetString(sel);
		selTrigger = str.ToStdString();
		resetAction();
	}
}

void Editor::onBox2Select(wxCommandEvent& event)
{
	int sel = lb2->GetSelection();
	if (sel != -1 && (operation == ACTOR || operation == OVERLAY))
	{
		wxString str = lb2->GetString(sel);
		selAction = str.ToStdString();
		resetCon();
	}
}

RoomEditor::RoomEditor() : wxFrame(NULL, wxID_ANY, selObject, wxDefaultPosition, wxSize(800, 600))
{
	rm = Room::objectMap[selObject];

	rmEdit = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE);
	rmEdit->SetScrollbars(20, 20, 5, 5);

	wxBoxSizer *vbs = new wxBoxSizer(wxVERTICAL);
	wxGridSizer *g1 = new wxGridSizer(1, 2, 2, 2);
	wxGridSizer *g2 = new wxGridSizer(1, 2, 2, 2);

	wxPanel *bPanel = new wxPanel(rmEdit, wxID_ANY);

	wxButton* defBtn = new wxButton(bPanel, DEF, wxT("Set As Default Room"));
	wxButton* folBtn = new wxButton(bPanel, FOL, wxT("Set Followed Object"));
	defBtn->Bind(wxEVT_BUTTON, &RoomEditor::onSetDefault, this);
	folBtn->Bind(wxEVT_BUTTON, &RoomEditor::onSetFollow, this);
	g1->Add(defBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	g1->Add(folBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	bPanel->SetSizer(g1);

	wxPanel *tPanel = new wxPanel(rmEdit, wxID_ANY);
	string defstr = "False";
	if (rm->is_default)
	{
		defstr = "True";
	}
	defTxt = new wxStaticText(tPanel, wxID_ANY, "Default Room : " + defstr);
	string folstr = "None";
	if (rm->getFollowedActor() != NULL)
	{
		folstr = rm->getFollowedActor()->getType()->name + " : " + to_string(rm->getFollowedActor()->getId());
	}
	folTxt = new wxStaticText(tPanel, wxID_ANY, "Camera Following : " + folstr);
	g2->Add(defTxt, 0, wxALIGN_CENTER | wxCENTER, 2);
	g2->Add(folTxt, 0, wxALIGN_CENTER | wxCENTER, 2);
	tPanel->SetSizer(g2);

	wxGridSizer *g3 = new wxGridSizer(1, 2, 2, 2);
	wxPanel *t2Panel = new wxPanel(rmEdit, wxID_ANY);
	actTxt = new wxStaticText(t2Panel, wxID_ANY, "Actors");
	ovTxt = new wxStaticText(t2Panel, wxID_ANY, "Overlays");
	g3->Add(actTxt, 0, wxALIGN_CENTER | wxCENTER, 2);
	g3->Add(ovTxt, 0, wxALIGN_CENTER | wxCENTER, 2);
	t2Panel->SetSizer(g3);

	wxGridSizer *g4 = new wxGridSizer(1, 2, 2, 2);
	wxGridSizer *g5 = new wxGridSizer(1, 3, 2, 2);
	wxGridSizer *g6 = new wxGridSizer(1, 3, 2, 2);
	wxPanel *btnsPanel = new wxPanel(rmEdit, wxID_ANY);
	wxPanel *actBtnPanel = new wxPanel(btnsPanel, wxID_ANY);
	wxPanel *ovBtnPanel = new wxPanel(btnsPanel, wxID_ANY);

	wxButton* actNewBtn = new wxButton(actBtnPanel, NEW_ITEM, wxT("New"));
	wxButton* actEditBtn = new wxButton(actBtnPanel, EDIT_ITEM, wxT("Edit"));
	wxButton* actDelBtn = new wxButton(actBtnPanel, DELETE_ITEM, wxT("Delete"));
	actNewBtn->Bind(wxEVT_BUTTON, &RoomEditor::onActNew, this);
	actEditBtn->Bind(wxEVT_BUTTON, &RoomEditor::onActEdit, this);
	actDelBtn->Bind(wxEVT_BUTTON, &RoomEditor::onActDelete, this);

	g5->Add(actNewBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	g5->Add(actEditBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	g5->Add(actDelBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	actBtnPanel->SetSizer(g5);

	wxButton* ovNewBtn = new wxButton(ovBtnPanel, NEW_ITEM, wxT("New"));
	wxButton* ovEditBtn = new wxButton(ovBtnPanel, EDIT_ITEM, wxT("Edit"));
	wxButton* ovDelBtn = new wxButton(ovBtnPanel, DELETE_ITEM, wxT("Delete"));
	ovNewBtn->Bind(wxEVT_BUTTON, &RoomEditor::onOvNew, this);
	ovEditBtn->Bind(wxEVT_BUTTON, &RoomEditor::onOvEdit, this);
	ovDelBtn->Bind(wxEVT_BUTTON, &RoomEditor::onOvDelete, this);

	g6->Add(ovNewBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	g6->Add(ovEditBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	g6->Add(ovDelBtn, 0, wxALIGN_CENTER | wxCENTER, 2);
	ovBtnPanel->SetSizer(g6);

	g4->Add(actBtnPanel, 0, wxEXPAND | wxLEFT, 2);
	g4->Add(ovBtnPanel, 0, wxEXPAND | wxRIGHT, 2);
	btnsPanel->SetSizer(g4);

	wxGridSizer *g7 = new wxGridSizer(1, 2, 2, 2);
	wxPanel *rmPanel = new wxPanel(rmEdit, wxID_ANY);
	actBox = new wxListBox(rmPanel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	for (auto& i : *rm->getActors())
	{
		string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
		actBox->Append(ap);
	}
	ovBox = new wxListBox(rmPanel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));
	for (auto& i : *rm->getOverlays())
	{
		string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
		ovBox->Append(ap);
	}
	g7->Add(actBox, 1, wxEXPAND | wxALL, 2);
	g7->Add(ovBox, 1, wxEXPAND | wxALL, 2);
	rmPanel->SetSizer(g7);

	vbs->Add(tPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	vbs->Add(bPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
	vbs->Add(t2Panel, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	vbs->Add(btnsPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
	vbs->Add(rmPanel, 1, wxEXPAND | wxALL);
	rmEdit->SetSizer(vbs);

	Centre();
}

void RoomEditor::onSetDefault(wxCommandEvent& event)
{
	wxArrayString tf;
	tf.Add("True");
	tf.Add("False");
	wxSingleChoiceDialog *defDialog = new wxSingleChoiceDialog(this, "Set Room as Default", "Choose True or False", tf);
	if (defDialog->ShowModal() == wxID_OK)
	{
		string str = defDialog->GetStringSelection().ToStdString();
		if (str == "False")
		{
			rm->is_default = false;
		}
		else
		{
			for (auto& pair : Room::objectMap)
			{
				if (pair.second->is_default == true)
				{
					pair.second->is_default = false;
				}
			}
			rm->is_default = true;
		}
	}
	string defstr = "False";
	if (rm->is_default)
	{
		defstr = "True";
	}
	defTxt->SetLabel("Default Room : " + defstr);
}
void RoomEditor::onSetFollow(wxCommandEvent& event)
{
	wxArrayString actorChoices;
	for (auto& i : *rm->getActors())
	{
		string str = i->getType()->name + " : " + to_string(i->getId());
		actorChoices.Add(str);
	}
	actorChoices.Add("None");
	wxSingleChoiceDialog *folDialog = new wxSingleChoiceDialog(this, "Set Actor for Camera to Follow", "Choose one from the list", actorChoices);
	if (folDialog->ShowModal() == wxID_OK)
	{
		string str = folDialog->GetStringSelection().ToStdString();
		if (str == "None")
		{
			rm->setFollowedActor(NULL);
		}
		else
		{
			for (auto& i : *rm->getActors())
			{
				string find = i->getType()->name + " : " + to_string(i->getId());
				if (find == str)
				{
					rm->setFollowedActor(i);
				}
			}
		}
		string folstr = "None";
		if (rm->getFollowedActor() != NULL)
		{
			folstr = rm->getFollowedActor()->getType()->name + " : " + to_string(rm->getFollowedActor()->getId());
		}
		folTxt->SetLabel("Camera Following : " + folstr);
	}
}

void RoomEditor::onActNew(wxCommandEvent& event)
{
	Actor* a;
	wxArrayString typeChoices;
	for (auto& pair : ActorType::objectMap)
	{
		typeChoices.Add(pair.first);
	}
	wxSingleChoiceDialog *actTypeChoice = new wxSingleChoiceDialog(this, "Choose Actor Type to Add", "Choose One and Press OK", typeChoices);
	if (actTypeChoice->ShowModal() == wxID_OK)
	{
		string str = actTypeChoice->GetStringSelection().ToStdString();
		ActorType* at = ActorType::objectMap[str];
		wxTextEntryDialog *actF1ChoiceDialog = new wxTextEntryDialog(this, "Enter X Position");
		if (actF1ChoiceDialog->ShowModal() == wxID_OK)
		{
			wxString str = actF1ChoiceDialog->GetValue();
			float spd1 = stof(str.ToStdString());
			wxTextEntryDialog *actF2ChoiceDialog = new wxTextEntryDialog(this, "Enter Y Position");
			if (actF2ChoiceDialog->ShowModal() == wxID_OK)
			{
				wxString str = actF2ChoiceDialog->GetValue();
				float spd2 = stof(str.ToStdString());
				State st = State(spd1, spd2);
				a = new Actor(rm, at, st);
				rm->addActor(a);
				string ap = a->getType()->name + " : " + to_string(a->getId()) + " : X : " + to_string(a->getStartState().xPosition) + " : Y : " + to_string(a->getStartState().yPosition);
				actBox->Append(ap);
			}
		}
	}
}
void RoomEditor::onActEdit(wxCommandEvent& event)
{
	int sel = actBox->GetSelection();
	if (sel != -1)
	{
		string str = actBox->GetStringSelection().ToStdString();
		bool found = false;
		for (auto& i : *rm->getActors())
		{
			string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
			if (ap == str)
			{
				wxTextEntryDialog *actF3ChoiceDialog = new wxTextEntryDialog(this, "Enter X Position");
				if (actF3ChoiceDialog->ShowModal() == wxID_OK)
				{
					wxString str = actF3ChoiceDialog->GetValue();
					float spd1 = stof(str.ToStdString());
					wxTextEntryDialog *actF4ChoiceDialog = new wxTextEntryDialog(this, "Enter Y Position");
					if (actF4ChoiceDialog->ShowModal() == wxID_OK)
					{
						wxString str = actF4ChoiceDialog->GetValue();
						float spd2 = stof(str.ToStdString());
						State st = State(spd1, spd2);
						i->setStartState(st);
						ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
						actBox->SetString(sel, ap);
					}
				}
				found = true;
			}
			if (found)
			{
				break;
			}
		}

	}
}
void RoomEditor::onActDelete(wxCommandEvent& event)
{
	int sel = actBox->GetSelection();
	if (sel != -1)
	{
		string str = actBox->GetStringSelection().ToStdString();
		bool found = false;
		for (auto& i : *rm->getActors())
		{
			string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
			if (ap == str)
			{
				rm->getActors()->remove(i);
				actBox->Delete(sel);
				found = true;
			}
			if (found)
			{
				break;
			}
		}

	}
}

void RoomEditor::onOvNew(wxCommandEvent& event)
{
	Overlay* o;
	wxArrayString typeChoices;
	for (auto& pair : OverlayType::objectMap)
	{
		typeChoices.Add(pair.first);
	}
	wxSingleChoiceDialog *ovTypeChoice = new wxSingleChoiceDialog(this, "Choose Overlay Type to Add", "Choose One and Press OK", typeChoices);
	if (ovTypeChoice->ShowModal() == wxID_OK)
	{
		string str = ovTypeChoice->GetStringSelection().ToStdString();
		OverlayType* ot = OverlayType::objectMap[str];
		wxTextEntryDialog *actF1ChoiceDialog = new wxTextEntryDialog(this, "Enter X Position");
		if (actF1ChoiceDialog->ShowModal() == wxID_OK)
		{
			wxString str = actF1ChoiceDialog->GetValue();
			float spd1 = stof(str.ToStdString());
			wxTextEntryDialog *actF2ChoiceDialog = new wxTextEntryDialog(this, "Enter Y Position");
			if (actF2ChoiceDialog->ShowModal() == wxID_OK)
			{
				wxString str = actF2ChoiceDialog->GetValue();
				float spd2 = stof(str.ToStdString());
				State st = State(spd1, spd2);
				o = new Overlay(rm, ot, st);
				rm->addOverlay(o);
				string ap = o->getType()->name + " : " + to_string(o->getId()) + " : X : " + to_string(o->getStartState().xPosition) + " : Y : " + to_string(o->getStartState().yPosition);
				ovBox->Append(ap);
			}
		}
	}
}
void RoomEditor::onOvEdit(wxCommandEvent& event)
{
	int sel = ovBox->GetSelection();
	if (sel != -1)
	{
		string str = ovBox->GetStringSelection().ToStdString();
		bool found = false;
		for (auto& i : *rm->getOverlays())
		{
			string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
			if (ap == str)
			{
				wxTextEntryDialog *ovF3ChoiceDialog = new wxTextEntryDialog(this, "Enter X Position");
				if (ovF3ChoiceDialog->ShowModal() == wxID_OK)
				{
					wxString str = ovF3ChoiceDialog->GetValue();
					float spd1 = stof(str.ToStdString());
					wxTextEntryDialog *ovF4ChoiceDialog = new wxTextEntryDialog(this, "Enter Y Position");
					if (ovF4ChoiceDialog->ShowModal() == wxID_OK)
					{
						wxString str = ovF4ChoiceDialog->GetValue();
						float spd2 = stof(str.ToStdString());
						State st = State(spd1, spd2);
						i->setStartState(st);
						ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
						ovBox->SetString(sel, ap);
					}
				}
				found = true;
			}
			if (found)
			{
				break;
			}
		}

	}
}
void RoomEditor::onOvDelete(wxCommandEvent& event)
{
	int sel = ovBox->GetSelection();
	if (sel != -1)
	{
		string str = ovBox->GetStringSelection().ToStdString();
		bool found = false;
		for (auto& i : *rm->getOverlays())
		{
			string ap = i->getType()->name + " : " + to_string(i->getId()) + " : X : " + to_string(i->getStartState().xPosition) + " : Y : " + to_string(i->getStartState().yPosition);
			if (ap == str)
			{
				rm->getOverlays()->remove(i);
				ovBox->Delete(sel);
				found = true;
			}
			if (found)
			{
				break;
			}
		}

	}
}