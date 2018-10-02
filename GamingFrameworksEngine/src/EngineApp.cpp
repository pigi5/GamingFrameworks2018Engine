#include "wx/wxprec.h"
#include "wx/splitter.h"
#include "wx/dcmirror.h"

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
	AUDIO = 8
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

wxEND_EVENT_TABLE()

// My frame constructor
MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, wxT("Frameworks Engine"),
		wxDefaultPosition, wxSize(800, 600),
		wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
{

	// Make a menubar
	wxMenu *menu = new wxMenu;

	menu->Append(NEW, wxT("&New"));
	menu->Append(OPEN, wxT("&Open"));
	menu->Append(SAVE, wxT("&Save"));
	menu->Append(QUIT, wxT("&Quit"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menu, wxT("&File"));

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
		currentPath = p + f;
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

// ----------------------------------------------------------------------------
// MySplitterWindow
// ----------------------------------------------------------------------------

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
	wxGridSizer *buttons = new wxGridSizer(4, 1, 20, 20);
	wxPanel *pnl = new wxPanel(this, wxID_ANY);
	wxButton *bt1 = new wxButton(pnl, SPRITE, "Sprites");
	wxButton *bt2 = new wxButton(pnl, AUDIO, "Audio");
	wxButton *bt3 = new wxButton(pnl, ROOM, "Rooms");
	wxButton *bt4 = new wxButton(pnl, OBJECT, "Objects");

	Connect(SPRITE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(Sidebar::onSprite));

	buttons->Add(bt1, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt2, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt3, 0, wxALIGN_CENTER | wxCENTER, 20);
	buttons->Add(bt4, 0, wxALIGN_CENTER | wxCENTER, 20);

	pnl->SetSizer(buttons);

	vszr->Add(pnl, 1, wxEXPAND);

	this->SetSizer(vszr);

	this->SetFocus();
	pnl->SetFocus();
	Centre();
}

void Sidebar::onSprite(wxCommandEvent& event)
{

}
void Sidebar::onAudio(wxCommandEvent& WXUNUSED(event))
{
	
}
void Sidebar::onObject(wxCommandEvent& WXUNUSED(event))
{
	
}
void Sidebar::onRoom(wxCommandEvent& WXUNUSED(event))
{
	
}

//Editor Window
Editor::Editor(wxWindow* parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetScrollbars(20, 20, 5, 5);
}