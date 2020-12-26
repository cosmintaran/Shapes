#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "Render/DeviceContext.h"
#include "Render/Camera.h"
#include <Widgets/Layer.h>
#include "core/Timestep.h"

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

private:
	SV::GS::DeviceContext* _canvas = nullptr;
	std::unique_ptr <SV::GS::OrotographicCamera> _camera;
	std::vector<SV::GS::Layer*> _layers;
	float _zoomLevel = 1.0f;
	bool _isDragged = false;
	const float _cameraMoveSpeed = 1.0f;
	float _aspectRatio;
	glm::vec3 _cameraPosition{0.0f};
	glm::ivec2 _lastMousePosition;

	wxMenuBar* m_pMenuBar = nullptr;
	wxMenu* m_pFileMenu = nullptr;
	wxMenu* m_pHelpMenu = nullptr;
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
	[[nodiscard]] glm::vec4 ScreenToClipSpace(float posX, float posY)const;
	[[nodiscard]] glm::vec4 ScreenToNdc(float posX, float posY)const;
	cMainWindow(const cMainWindow& tw) = delete;
	cMainWindow(cMainWindow&& tw) = delete;
	cMainWindow& operator=(const cMainWindow& tw) = delete;
	cMainWindow& operator=(cMainWindow&&) = delete;

};

