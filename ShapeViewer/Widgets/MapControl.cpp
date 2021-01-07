#include "stdafx.h"
#include "MapControl.h"
#include "core/Id.h"
#include "Widgets/Shapes/Polygon.h"
#include <boost/geometry/algorithms/within.hpp>
#include <wx/sizer.h>
namespace SV {

	wxDEFINE_EVENT(OnMapMouseMove, wxCommandEvent);

	MapControl::MapControl(wxWindow* parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
		wxPanel(parent, winid,  pos, size, style, name)
	{

		_envelope.MaxX = size.GetWidth() / size.GetHeight();
		_envelope.MaxY = 1.0;
		_envelope.MinX = -(size.GetWidth() / size.GetHeight());
		_envelope.MinY = -1.0;

		_cameraMoveSpeed = (_envelope.MaxX - _envelope.MinX) > 0 ? (_envelope.MaxX - _envelope.MinX) : 2.0f;

		_camera = std::make_unique<SV::GS::OrotographicCamera>(_envelope.MinX * _zoomLevel, _envelope.MaxX * _zoomLevel, _envelope.MinY * _zoomLevel, _envelope.MaxY * _zoomLevel, -1.0f, 10.0f);
		_canvas = new SV::GS::DrawingContext(this, Window::ID::GLCONTEXT, nullptr, wxDefaultPosition, size);

		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(_canvas, 0);
		SetSizerAndFit(sizer);

		this->Bind(wxEVT_SIZE, &MapControl::OnSize, this);
		_canvas->Bind(wxEVT_MOUSEWHEEL, &MapControl::OnScroll, this);
		_canvas->Bind(wxEVT_LEFT_DOWN, &MapControl::OnLeftMouseBttDown, this);
		_canvas->Bind(wxEVT_LEFT_UP, &MapControl::OnLeftMouseBttUp, this);
		_canvas->Bind(wxEVT_RIGHT_UP, &MapControl::OnRightMouseBttUp, this);
		_canvas->Bind(wxEVT_MOTION, &MapControl::OnMouseMove, this);
		_canvas->Bind(wxEVT_MIDDLE_DCLICK, &MapControl::OnScrollMouseDoubleClicked, this);
		_canvas->Bind(wxEVT_KEY_UP, &MapControl::OnKeyUp, this);

		SetBackgroundColour(wxColor(255, 0, 255));
	}

	MapControl::~MapControl()
	{
		for (auto& l : _layers) {
			delete l;
			l = nullptr;
		}
		_layers.clear();
	}

	void MapControl::Paint(SV::CORE::Timestep ts)
	{
		bool isPoisitionChanged = false;

		if (wxGetKeyState(wxKeyCode::WXK_LEFT)) {
			_cameraPosition.x += _cameraMoveSpeed * ts;
			isPoisitionChanged = true;
		}

		if (wxGetKeyState(wxKeyCode::WXK_RIGHT)) {
			_cameraPosition.x -= _cameraMoveSpeed * ts;
			isPoisitionChanged = true;
		}

		if (wxGetKeyState(wxKeyCode::WXK_UP)) {
			_cameraPosition.y -= _cameraMoveSpeed * ts;
			isPoisitionChanged = true;
		}

		if (wxGetKeyState(wxKeyCode::WXK_DOWN)) {
			_cameraPosition.y += _cameraMoveSpeed * ts;
			isPoisitionChanged = true;
		}

		if (isPoisitionChanged)
			_camera->SetPosition(_cameraPosition);

		Paint();
	}

	void MapControl::LoadEsriShapeFile(const std::filesystem::path& filePath)
	{
		SV::EsriShpLayer* lay = new SV::EsriShpLayer(filePath.filename().string().c_str(), filePath.string().c_str(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));

		if (_layers.size() == 0) {
			_envelope = lay->GetEnvelope();

			_zoomLevel = 1.2f;

			const float translateX = ((_envelope.MaxX - _envelope.MinX) * 0.5f) + _envelope.MinX;
			const float translateY = ((_envelope.MaxY - _envelope.MinY) * 0.5f) + _envelope.MinY;

			const float left = ((_envelope.MinX - translateX) * _zoomLevel) + translateX;
			const float right = ((_envelope.MaxX - translateX) * _zoomLevel) + translateX;

			const float bottom = ((_envelope.MinY - translateY) * _zoomLevel) + translateY;
			const float top = ((_envelope.MaxY - translateY) * _zoomLevel) + translateY;

			_camera->SetProjection(left, right, bottom, top);
			_cameraPosition = glm::vec3(0.0f);
			_camera->SetPosition(_cameraPosition);
			_cameraMoveSpeed = (right - left) < 0 ? 2.0f : (right - left);
		}
		_layers.push_back(lay);
	}

	void MapControl::LoadCgxmlFile(const std::filesystem::path& filePath)
	{
	}

	void MapControl::LoadCgxmlFiles(const std::filesystem::path& folderPath)
	{
	}

	void MapControl::OnScroll(wxMouseEvent& event)
	{
		float zoomLevel = (float)(_zoomLevel - (float)event.GetWheelRotation() * 0.0015 * _zoomLevel);

		if (zoomLevel <= 0.00001 || zoomLevel > 10) return;
		_zoomLevel = zoomLevel;

		const glm::vec3 beforeZoom = ScreenToClipSpace(event.GetX(), event.GetY());

		const float translateX = ((_envelope.MaxX - _envelope.MinX) * 0.5f) + _envelope.MinX;
		const float translateY = ((_envelope.MaxY - _envelope.MinY) * 0.5f) + _envelope.MinY;

		const float left = ((_envelope.MinX - translateX) * _zoomLevel) + translateX;
		const float right = ((_envelope.MaxX - translateX) * _zoomLevel) + translateX;

		const float bottom = ((_envelope.MinY - translateY) * _zoomLevel) + translateY;
		const float top = ((_envelope.MaxY - translateY) * _zoomLevel) + translateY;

		_camera->SetProjection(left, right, bottom, top);
		_cameraMoveSpeed = (right - left) < 0 ? 2.0f : (right - left);
		const glm::vec3 afterZoom = ScreenToClipSpace(event.GetX(), event.GetY());

		_camera->SetPosition(_camera->GetPosition() + (beforeZoom - afterZoom));
	}

	void MapControl::OnSize(wxSizeEvent& event)
	{
		_canvas->Resize(event.GetSize());
	}

	void MapControl::OnLeftMouseBttDown(wxMouseEvent& event)
	{
		wxSetCursor(wxCursor(wxCURSOR_HAND));
		_isDragged = true;
		_lastMousePosition = { event.GetX(), event.GetY(), 0.0f };
	}

	void MapControl::OnLeftMouseBttUp(wxMouseEvent& event)
	{
		if (_isDragged) {
			wxSetCursor(wxCursor(wxCURSOR_ARROW));
			_isDragged = false;
		}
	}

	void MapControl::OnRightMouseBttUp(wxMouseEvent& event)
	{
		const glm::vec3 currentMousePosition = ScreenToClipSpace(event.GetX(), event.GetY());
		boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> pt(currentMousePosition.x, currentMousePosition.y);

		for (size_t i = 0; i < _layers.size(); ++i) {

			if (_layers[i]->IsVisible() && _layers[i]->IsPointInLayer(currentMousePosition)) {

				for (size_t j = 0; j < _layers[i]->Count(); ++j) {
					SV::Shapes::Polygon* yy = (SV::Shapes::Polygon*)(*_layers[i])[j];

					if (boost::geometry::within(pt, yy->GetBoostPolygon()) == false) continue;
					yy->SetSelected(!yy->IsSelected());
				}
			}
		}
	}

	void MapControl::OnMouseMove(wxMouseEvent& event)
	{
		const glm::vec3 coordinates = ScreenToClipSpace(event.GetX(), event.GetY());

		wxCommandEvent myEvent(OnMapMouseMove, GetId());
		myEvent.SetEventObject(this);
		myEvent.SetString(wxString::Format(wxT("%f %f"), coordinates.x, coordinates.y));
		ProcessWindowEvent(myEvent);

		if (_isDragged == false) return;
		auto state = wxGetMouseState();
		if (state.LeftIsDown()) {
			wxSetCursor(wxCursor(wxCURSOR_HAND));
			long x = 0, y = 0;
			event.GetPosition(&x, &y);
			const glm::vec3 currentMousePosition = { x, y, 0.0f };
			Translate(currentMousePosition);
		}
	}

	void MapControl::OnScrollMouseDoubleClicked(wxMouseEvent& event)
	{
		_zoomLevel = 1.0f;
		_camera->SetProjection(_envelope.MinX, _envelope.MaxX, _envelope.MinY, _envelope.MaxY);
		_cameraMoveSpeed = (_envelope.MaxX - _envelope.MinX) > 0 ? (_envelope.MaxX - _envelope.MinX) : 2.0f;
		_cameraPosition = glm::vec3(0.0f);
		_camera->SetPosition(_cameraPosition);
	}

	void MapControl::OnKeyUp(wxKeyEvent& event)
	{
		const float speed = 0.01f;
		switch (event.GetKeyCode())
		{
		case WXK_LEFT:

			_camera->SetPosition(_camera->GetPosition() + glm::vec3(speed, 0.0f, 0.0f));

			break;

		case WXK_RIGHT:
			_camera->SetPosition(_camera->GetPosition() + glm::vec3(-speed, 0.0f, 0.0f));
			break;

		case WXK_UP:
			_camera->SetPosition(_camera->GetPosition() + glm::vec3(0.0f, -speed, 0.0f));
			break;

		case WXK_DOWN:
			_camera->SetPosition(_camera->GetPosition() + glm::vec3(0.0f, speed, 0.0f));
			break;

		default:
			break;
		}
	}

	glm::vec3 MapControl::ScreenToClipSpace(float posX, float posY) const
	{
		glm::vec3 projected(posX, (float)GetSize().GetHeight() - posY, 1.0f);
		glm::vec4 viewport(0, 0, (float)GetSize().GetWidth(), (float)GetSize().GetHeight());
		glm::vec3 coords = glm::unProject(projected, _camera->GetViewMatrix(), _camera->GetProjectionMatrix(), viewport);
		return coords;
	}

	void MapControl::Translate(glm::vec3 translation)
	{

		const double widthRatio = (_envelope.MaxX - _envelope.MinX) / (double)GetSize().GetWidth();
		const double heighRatio = (_envelope.MaxY - _envelope.MinY) / (double)GetSize().GetHeight();
		
		const glm::vec3 delta = translation - _lastMousePosition;

		_cameraPosition.x -= delta.x * widthRatio;
		_cameraPosition.y += delta.y * heighRatio;

		_camera->SetPosition(_cameraPosition);
		_lastMousePosition = translation;
	}

	glm::vec3 MapControl::ScreenToNdc(float posX, float posY) const
	{
		const glm::vec3 ndcCoords(posX / (GetSize().GetWidth() * 0.5f) - 1, (GetSize().GetHeight() - posY) / (GetSize().GetHeight() * 0.5f) - 1, 0.0f);
		return ndcCoords;
	}

	void MapControl::Paint()
	{
		_canvas->ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		_canvas->UpdateScene(_camera->GetViewProjectionMatrix());

		for (size_t i = 0; i < _layers.size(); ++i) {
			if (_layers[i] != nullptr && _layers[i]->IsVisible()) {
				_layers[i]->Draw(*_canvas);
				_layers[i]->SetIsDirty(false);
			}
		}
		_canvas->EndScene();
	}

}

