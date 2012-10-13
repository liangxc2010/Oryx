#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/aui/aui.h"

// The frame containing the GL canvas
class MainFrame : public wxFrame
{
public:
	MainFrame(wxFrame *frame,
		const wxString& title = wxT("Oryx"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE);

	virtual ~MainFrame();
private :
	void OnExit(wxCommandEvent& event);
	wxAuiDockArt* GetDockArt();
	void DoUpdate();

	void OnWindowScene(wxCommandEvent& event);
	void OnWindowGame(wxCommandEvent& event);

private:
	wxAuiManager m_mgr;

	DECLARE_EVENT_TABLE()
};


#endif // __MAINFRAME_H__