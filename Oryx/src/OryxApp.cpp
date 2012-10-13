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

#include "OryxApp.h"
#include "MainFrame.h"

#include "wxlua/wxlua.h"

#include "wxbind/include/wxbinddefs.h"
WXLUA_DECLARE_BIND_ALL

IMPLEMENT_APP(OryxApp)

wxLuaState* state;
//extern int wxluatype_wxFrame;

bool OryxApp::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	WXLUA_IMPLEMENT_BIND_ALL

	wxLuaState::sm_wxAppMainLoop_will_run = true;

	state = new wxLuaState(this, wxID_ANY);
	if (!state->Ok())
		return false;
	
	lua_State* L = state->GetLuaState();
	
	MainFrame* frame = new MainFrame(NULL);

	int type = wxluaT_gettype(L, "wxFrame");
	//bool b = state->wxluaT_PushUserDataType(frame, wxluatype_wxFrame, true);
	bool b = state->wxluaT_PushUserDataType(frame, type, true);
	int top = state->lua_GetTop();
	lua_setglobal(L,"frame");

	//int ret = state->RunFile(wxT(".\\scripts\\test.lua"));
	
	int ret = luaL_dofile(L, "scripts\\test.lua");
	if (ret)
	{
		int top = state->lua_GetTop();
		const char* err = lua_tostring(L, -1);
		//printf(err);
		wxString msg = wxString(err);
		wxMessageBox(msg);
		return false;
	}

	

	return true;
}

int OryxApp::OnExit()
{
	state->CloseLuaState(true);
	state->Destroy();
	delete state;

	return wxApp::OnExit();
}