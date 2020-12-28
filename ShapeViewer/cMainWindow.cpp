#include "stdafx.h"
#include "cMainWindow.h"
#include "core/Id.h"
#include "Widgets/Shapes/ShpTypes.h"
#include "core/Log.h"
#include <wx/progdlg.h>

BEGIN_EVENT_TABLE(cMainWindow, wxFrame)
EVT_CLOSE(cMainWindow::OnClose)
END_EVENT_TABLE()


cMainWindow::cMainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name), _aspectRatio(0.0f)
{
	_envelope.MaxX =  1.0;
	_envelope.MaxY =  1.0;
	_envelope.MinX = -1.0;
	_envelope.MinY = -1.0;
	InitializeUIComponents(size);

	
}

void cMainWindow::InitializeUIComponents(const wxSize& size)
{

	m_pMenuBar = new wxMenuBar();
	// File Menu
	m_pFileMenu = new wxMenu();
	m_pFileMenu->Append(wxID_OPEN, _T("&Open"));
	m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
	m_pFileMenu->AppendSeparator();
	m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
	m_pMenuBar->Append(m_pFileMenu, _T("&File"));
	// About menu
	m_pHelpMenu = new wxMenu();
	m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
	m_pMenuBar->Append(m_pHelpMenu, _T("&Help"));

	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(cMainWindow::OnMenuOpenCmd));

	SetMenuBar(m_pMenuBar);

	CreateStatusBar(4);
	SetStatusText(wxT("0.00000 0.00000 "), 0);
	SetStatusText(wxT("                "), 2);
	auto m_statusbar = this->GetStatusBar();
	wxRect rect;
	m_statusbar->GetFieldRect(3, rect);
	m_statusbar->Fit();

	 gauge = new wxGauge(m_statusbar, wxID_ANY, 100, rect.GetPosition(), rect.GetSize(), wxGA_SMOOTH);
	_aspectRatio = ((float)size.GetWidth() / (float)size.GetHeight());
	float bottom = ((_envelope.MaxX - _envelope.MinX) * 0.5) * _aspectRatio * _zoomLevel;
	float top = ((_envelope.MaxY - _envelope.MinY) * 0.5) * _aspectRatio * _zoomLevel;
	_camera = std::make_unique<SV::GS::OrotographicCamera>(-bottom, bottom, -top, top, -1.0f, 10.0f);
	_canvas = new SV::GS::DeviceContext(this, Window::ID::GLCONTEXT, nullptr, { 0, 0 }, size);

	Fit();
	Centre();

	this->Bind(wxEVT_SIZE, &cMainWindow::OnSize, this);
	_canvas->Bind(wxEVT_MOUSEWHEEL, &cMainWindow::OnScroll, this);
	_canvas->Bind(wxEVT_LEFT_DOWN, &cMainWindow::OnLeftMouseBttDown, this);
	_canvas->Bind(wxEVT_LEFT_UP, &cMainWindow::OnLeftMouseBttUp, this);
	_canvas->Bind(wxEVT_RIGHT_UP, &cMainWindow::OnRightMouseBttUp, this);
	_canvas->Bind(wxEVT_MOTION, &cMainWindow::OnMouseMove, this);
	_canvas->Bind(wxEVT_MIDDLE_DCLICK, &cMainWindow::OnScrollMouseDoubleClicked, this);
	_canvas->Bind(wxEVT_KEY_UP, &cMainWindow::OnKeyUp, this);
}


cMainWindow::~cMainWindow()
{
	for (auto& l : _layers) {
		delete l;
		l = nullptr;
	}
	_layers.clear();
}


void cMainWindow::vPaint(SV::CORE::Timestep ts)
{

	if (_canvas == nullptr || !IsShownOnScreen()) return;

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

void cMainWindow::Paint()
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

void cMainWindow::OnScroll(wxMouseEvent& event)
{
	float zoomLevel = (float)(_zoomLevel - (float)event.GetWheelRotation() * 0.0016 * _zoomLevel);

	const glm::vec4 beforeZoom = ScreenToClipSpace(event.GetX(), event.GetY());
	_zoomLevel = zoomLevel;

	const float left = static_cast<float>((float)_envelope.MinX / _zoomLevel);
	const float bottom = static_cast<float>((float)_envelope.MinY / _zoomLevel);
	const float right = static_cast<float>(_envelope.MaxX * _aspectRatio * _zoomLevel);
	const float top = static_cast<float>(_envelope.MaxY * _aspectRatio * _zoomLevel);
	_camera->SetProjection(left, right, bottom, top);

	//const glm::vec4 afterZoom = ScreenToClipSpace(event.GetX(), event.GetY());
	//const glm::vec4 diff = beforeZoom - afterZoom;
	//_camera->SetPosition(_camera->GetPosition() + glm::vec3(diff.x, diff.y, 0.0f));
}

void cMainWindow::OnSize(wxSizeEvent& event)
{
	wxFrame::OnSize(event);
	_aspectRatio = (float)event.GetSize().GetWidth() / (float)event.GetSize().GetHeight();
	_canvas->Resize(event.GetSize());
	_camera->SetProjection((_aspectRatio * _envelope.MinX * _zoomLevel), (_aspectRatio * _envelope.MaxX * _zoomLevel), _envelope.MinY * _zoomLevel, _envelope.MaxY * _zoomLevel);

}

void cMainWindow::OnLeftMouseBttDown(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(wxCURSOR_HAND));
	_isDragged = true;
	event.GetPosition(&_lastMousePosition.x, &_lastMousePosition.y);
}

void cMainWindow::OnLeftMouseBttUp(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
	_isDragged = false;
}

void cMainWindow::OnRightMouseBttUp(wxMouseEvent& event)
{
	//std::for_each(_layers.cend(), _layers.cbegin(), [&](const SV::GS::Layer& layer) {
	//	if (layer.IsVisible()) {

	//	}
	//	});
}

void cMainWindow::OnMouseMove(wxMouseEvent& event)
{
	const glm::vec4 coordinates = ScreenToClipSpace(event.GetX(), event.GetY());
	SetStatusText(wxString::Format(wxT("%f %f"), coordinates.x, coordinates.y), 0);

	if (_isDragged == false) return;

	wxSetCursor(wxCursor(wxCURSOR_HAND));
	auto state = wxGetMouseState();
	if (state.LeftIsDown()) {
		long x = 0, y = 0;
		event.GetPosition(&x, &y);

		const float deltaX = x - _lastMousePosition.x;
		const float deltaY = y - _lastMousePosition.y;

		const float ratio = _aspectRatio * _zoomLevel;
		_cameraPosition.x += ((ratio * deltaX) / this->GetSize().GetWidth()) * -1;
		_cameraPosition.y += ((ratio * deltaY) / this->GetSize().GetHeight());

		_camera->SetPosition(_cameraPosition);

		_lastMousePosition.x = x;
		_lastMousePosition.y = y;
	}
}

void cMainWindow::OnScrollMouseDoubleClicked(wxMouseEvent& event)
{
	_zoomLevel = 1.0f;
	_camera->SetProjection((_aspectRatio * _envelope.MinX * _zoomLevel), (_aspectRatio * _envelope.MaxX * _zoomLevel), _envelope.MinY * _zoomLevel, _envelope.MaxY * _zoomLevel);
	_cameraPosition = glm::vec3((_aspectRatio * _envelope.MaxX - _aspectRatio * _envelope.MinX) *0.5, (_envelope.MaxY - _envelope.MinY) * 0.5, 0.0f);
	_camera->SetPosition(_cameraPosition);
}

void cMainWindow::OnKeyUp(wxKeyEvent& event)
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

void cMainWindow::OnMenuOpenCmd(wxCommandEvent& WXUNUSED)
{
	wxFileDialog openFileDialog(this, "Select a shapefile", "", "", "shp files (*.shp)|*.shp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK) {

		SetStatusText(wxT("Loading shp file"), 3);

		for (int i = 0; i < 100; ++i) {
			//if (!dialog.Update(i)) {
			//	// Cancelled by user.
			//	break;
			//}

		}

		SV::GS::EsriShpLayer* lay = new SV::GS::EsriShpLayer(openFileDialog.GetFilename().c_str().AsChar(), openFileDialog.GetPath().c_str().AsChar(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));
		gauge->Pulse();
		if (lay->ReadShapeFile()) {
			if (_layers.size() == 0){
				_envelope = lay->GetEnvelope();
				_zoomLevel = 1.0f;

				const float left   = static_cast<float>(_envelope.MinX * _zoomLevel);
				const float bottom = static_cast<float>(_envelope.MinY * _zoomLevel);
				const float right  = static_cast<float>(_envelope.MaxX * _aspectRatio * _zoomLevel);
				const float top    = static_cast<float>(_envelope.MaxY * _aspectRatio * _zoomLevel);
				_camera->SetProjection(left, right, bottom, top);
				_cameraPosition = glm::vec3((right-left) *0.5, (top-bottom)*0.5, 0.0f);
				_camera->SetPosition(_cameraPosition);
			}
			_layers.push_back(lay);
		}
	}

	SetStatusText(wxT(""), 3);
	gauge->SetValue(0);
}

void cMainWindow::OnClose(wxCloseEvent& event)
{
	for (auto& l : _layers) {
		delete l;
		l = nullptr;
	}
	_layers.clear();

	event.Skip();
}

glm::vec4 cMainWindow::ScreenToClipSpace(float posX, float posY) const
{
	const glm::vec4 ndcCoords = ScreenToNdc(posX, posY);
	const glm::vec4 translatedNDC(_camera->GetPosition().x, _camera->GetPosition().y, 0.0f, 0.0f);
	const glm::vec4 coords = _camera->GetInversProjection() * ndcCoords;
	return coords + translatedNDC;
}

glm::vec4 cMainWindow::ScreenToNdc(float posX, float posY) const
{
	const glm::vec4 ndcCoords(posX / (GetSize().GetWidth() * 0.5f) - 1, (GetSize().GetHeight() - posY) / (GetSize().GetHeight() * 0.5f) - 1, 0.0f, 0.0f);
	return ndcCoords;
}
