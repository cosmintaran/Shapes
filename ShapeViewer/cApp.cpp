#include "stdafx.h"
#include "cApp.h"
#include "core/Id.h"
#include "core/Log.h"
#include <profileapi.h>

constexpr double FPS = 1.0 / 10.0;

IMPLEMENT_APP(cApp)

cApp::cApp()
    :window(nullptr)
{
    _lastFrameTime.QuadPart = 0;
}

cApp::~cApp()
{
#ifdef _DEBUG
    FreeConsole();
#endif
   // delete window;  
}

bool cApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

#ifdef _DEBUG
    AllocConsole();
    SV::CORE::Log::Init();
#endif

    window = new cMainWindow(nullptr, Window::ID::MAINWINDOW, _("Shape Viewer"), { 0,0 }, {800,600});
    window->Show();
    activateRenderLoop(true);
    return true;
}

void cApp::onIdle(wxIdleEvent& evt)
{
    LARGE_INTEGER time, fr;
    QueryPerformanceFrequency(&fr);
    QueryPerformanceCounter(&time);
    
    if (render_loop_on)
    {
        SV::CORE::Timestep step(((double)time.QuadPart - (double)_lastFrameTime.QuadPart) / (double)fr.QuadPart);
        window->vPaint(step);

        if (step < FPS) {
            Sleep((FPS - step)*1000);
        }
        evt.RequestMore(); // render continuously, not only once on idle
        _lastFrameTime = time;
    }
}

void cApp::activateRenderLoop(bool on)
{
    if (on && !render_loop_on)
    {
        Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(cApp::onIdle));
        render_loop_on = true;
    }
    else if (!on && render_loop_on)
    {
        Disconnect(wxEVT_IDLE, wxIdleEventHandler(cApp::onIdle));
        render_loop_on = false;
    }
}


