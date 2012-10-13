#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "wx/glcanvas.h"
#include "wx/timer.h"
#include "wx/math.h"
#include "wx/log.h"
#include "wx/cmdline.h"
#include "wx/wfstream.h"
#include "wx/zstream.h"
#include "wx/txtstrm.h"

#include "MainFrame.h"
//#include "../sample.xpm"

#include "TestGLCanvas.cpp"

static TestGLCanvas *canvas1;
static TestGLCanvas *canvas2;

static wxAuiNotebook* ctrl;

enum
{
	MenuID_Scene = 0,
	MenuID_Game,
};

//---------------------------------------------------------------------------
// MainFrame
//---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(MenuID_Scene, MainFrame::OnWindowScene)
	EVT_MENU(MenuID_Game, MainFrame::OnWindowGame)
END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
	const wxSize& size, long style)
	: wxFrame(frame, wxID_ANY, title, pos, size, style)
{
	//SetIcon(wxICON(sample));
	m_mgr.SetManagedWindow(this);

	// Make a menubar
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append( wxID_EXIT, wxT( "E&xit" ) );

	wxMenu *window = new wxMenu;
	window->Append(MenuID_Scene, wxT("S&cene"));
	window->Append(MenuID_Game, wxT("G&ame"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( fileMenu, wxT( "&File" ) );
	menuBar->Append( window, wxT( "&Window" ) );
	SetMenuBar( menuBar );

	wxSize client_size = GetClientSize();

	ctrl = new wxAuiNotebook(this, wxID_ANY,
		wxPoint(client_size.x, client_size.y),
		wxSize(640,480),
		wxAUI_NB_DEFAULT_STYLE);
	ctrl->Freeze();

	canvas1 = new TestGLCanvas(ctrl);
	canvas2 = new TestGLCanvas(ctrl);

	ctrl->AddPage(canvas1, wxT("Scene"));
	ctrl->AddPage(canvas2, wxT("Game"));

	ctrl->Thaw();

	m_mgr.AddPane(ctrl, wxAuiPaneInfo().Name(wxT("Center")).
				CenterPane().PaneBorder(false));


	m_mgr.AddPane( new wxStaticText( this, wxID_ANY, wxT("Pane Border Size:") ), wxAuiPaneInfo().
		Name(wxT("settings2")).Caption(wxT("Dock Manager Settings2")).
		Right().Layer(1).Position(1).
		CloseButton(true).MaximizeButton(true));

// 	m_mgr.AddPane( new wxPanel( ctrl, wxID_ANY ), wxAuiPaneInfo().Name(wxT("Center")).
// 		CenterPane().PaneBorder(false));

	wxString Perspective = m_mgr.SavePerspective();
	// "commit" all changes made to wxAuiManager
	m_mgr.Update();

	// Show the frame
	Maximize(true);
	Show(true);

	
}

MainFrame::~MainFrame()
{
	m_mgr.UnInit();
}

// Intercept menu commands
void MainFrame::OnExit( wxCommandEvent& WXUNUSED(event) )
{
	// true is to force the frame to close
	Close(true);
}

void MainFrame::OnWindowScene( wxCommandEvent& WXUNUSED(event) )
{
	ctrl->Freeze();

	canvas1 = new TestGLCanvas(ctrl);

	ctrl->AddPage(canvas1, wxT("Scene"));

	ctrl->Thaw();
}

void MainFrame::OnWindowGame( wxCommandEvent& WXUNUSED(event) )
{
	ctrl->Freeze();

	canvas2 = new TestGLCanvas(ctrl);

	ctrl->AddPage(canvas2, wxT("Game"));

	ctrl->Thaw();
}

wxAuiDockArt* MainFrame::GetDockArt()
{
	return m_mgr.GetArtProvider();
}

void MainFrame::DoUpdate()
{
	m_mgr.Update();
}