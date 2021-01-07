#pragma once

#include <wx/panel.h>

#include "Render/DeviceContext.h"
#include "Render/Camera.h"
#include "core/Timestep.h"
#include "Widgets/EsriShpLayer.h"
#include "Widgets/Envelope.h"
#include <filesystem>

namespace SV {

    wxDECLARE_EVENT(OnMapMouseMove, wxCommandEvent);

	class MapControl
		: public wxPanel {

	public :
        MapControl(wxWindow* parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxASCII_STR(wxPanelNameStr));
        ~MapControl();
        void Paint(SV::CORE::Timestep timeStep);
        void LoadEsriShapeFile(const std::filesystem::path& filePath);
        void LoadCgxmlFile(const std::filesystem::path& filePath);
        void LoadCgxmlFiles(const std::filesystem::path&  folderPath);

	private:
        SV::GS::DrawingContext* _canvas = nullptr;
        std::unique_ptr <SV::GS::OrotographicCamera> _camera;
        std::vector<SV::Layer*> _layers;
        float _zoomLevel = 1.0f;
        bool _isDragged = false;
        float _cameraMoveSpeed = 0.0f;
        glm::vec3 _cameraPosition{ 0.0f };
        glm::vec3 _lastMousePosition;
        SV::Envelope _envelope;

    private:

        void OnScroll(wxMouseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnLeftMouseBttDown(wxMouseEvent& event);
        void OnLeftMouseBttUp(wxMouseEvent& event);
        void OnRightMouseBttUp(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnScrollMouseDoubleClicked(wxMouseEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        void Translate(glm::vec3 translation);

        [[nodiscard]] glm::vec3 ScreenToClipSpace(float posX, float posY)const;       
        [[nodiscard]] glm::vec3 ScreenToNdc(float posX, float posY)const;
        void Paint();
	};

}
