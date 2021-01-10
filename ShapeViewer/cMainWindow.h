#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "core/Timestep.h"
#include "Widgets/MapControl.h"
#include "Widgets/LayerControl.h"

#pragma comment(lib, "OpenGL32.lib")


class cMainWindow : public wxFrame
{

public:
    cMainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
    ~cMainWindow();
    void vPaint(const SV::CORE::Timestep& ts);
    DECLARE_EVENT_TABLE()

private:
    bool _isDataLoading = false;
    SV::MapControl* _map = nullptr;
    SV::LayerControl* _layerCtrl = nullptr;
    LARGE_INTEGER startTimeFPS;
    LARGE_INTEGER finalTimeFPS;
    LARGE_INTEGER frequency;
    int frameCount = 0;

private:

    void InitializeUIComponents(const wxSize& size);
    void OnMenuOpenCmd(wxCommandEvent& WXUNUSED(event));
    void OnCoordsUpdatedCmd(wxCommandEvent& ev);
    void OnMenuExitCmd(wxCommandEvent& ev);
    cMainWindow(const cMainWindow& tw) = delete;
    cMainWindow(cMainWindow&& tw) = delete;
    cMainWindow& operator=(const cMainWindow& tw) = delete;
    cMainWindow& operator=(cMainWindow&&) = delete;

};

