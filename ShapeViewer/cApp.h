#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include"cMainWindow.h"


class cApp : public wxApp {

public:
	cApp();
	~cApp();
	bool OnInit();
	void onIdle(wxIdleEvent& evt);
	void activateRenderLoop(bool on);

private:
	bool render_loop_on = false;
	LARGE_INTEGER _lastFrameTime;
	cMainWindow* window;

};

DECLARE_APP(cApp); //wxGetApp()