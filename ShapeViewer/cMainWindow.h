#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "Render/DeviceContext.h"
#include "Render/Camera.h"
#include <Widgets/EsriShpLayer.h>
#include "core/Timestep.h"
#include <Widgets/Envelope.h>

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
    void vPaint(SV::CORE::Timestep );
    DECLARE_EVENT_TABLE()

private:
    SV::GS::DeviceContext* _canvas = nullptr;
    std::unique_ptr <SV::GS::OrotographicCamera> _camera;
    std::vector<SV::GS::ILayer*> _layers;
    float _zoomLevel = 1.0f;
    bool _isDragged = false;
    const float _cameraMoveSpeed = 2.0f;
    glm::vec3 _cameraPosition{0.0f};
    glm::vec3 _lastMousePosition;
    SV::Envelope _envelope;
    LARGE_INTEGER startTimeFPS;
    LARGE_INTEGER finalTimeFPS;
    LARGE_INTEGER frequency;
    int frameCount = 0;

    wxMenuBar* m_pMenuBar = nullptr;
    wxMenu* m_pFileMenu = nullptr;
    wxMenu* m_pHelpMenu = nullptr;
    wxGauge* gauge = nullptr;
private:

    void InitializeUIComponents(const wxSize& size);
    void Paint();
    void OnScroll(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnLeftMouseBttDown(wxMouseEvent& event);
    void OnLeftMouseBttUp(wxMouseEvent& event);
    void OnRightMouseBttUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnScrollMouseDoubleClicked(wxMouseEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnMenuOpenCmd(wxCommandEvent& WXUNUSED(event));
    void OnClose(wxCloseEvent& event);
    [[nodiscard]] glm::vec3 ScreenToClipSpace(float posX, float posY)const;
    [[nodiscard]] glm::vec3 ScreenToNdc(float posX, float posY)const;
    cMainWindow(const cMainWindow& tw) = delete;
    cMainWindow(cMainWindow&& tw) = delete;
    cMainWindow& operator=(const cMainWindow& tw) = delete;
    cMainWindow& operator=(cMainWindow&&) = delete;

};

