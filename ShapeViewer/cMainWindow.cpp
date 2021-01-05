#include "stdafx.h"
#include "cMainWindow.h"
#include "core/Id.h"
#include "Widgets/Shapes/ShpTypes.h"
#include <wx/progdlg.h>
#include "Widgets/Shapes/Polygon.h"
#include <boost/geometry/algorithms/within.hpp> 

BEGIN_EVENT_TABLE(cMainWindow, wxFrame)
EVT_CLOSE(cMainWindow::OnClose)
END_EVENT_TABLE()


cMainWindow::cMainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name)
{
	_envelope.MaxX = size.GetWidth() / size.GetHeight();
	_envelope.MaxY = 1.0;
	_envelope.MinX = -(size.GetWidth() / size.GetHeight());
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
	//auto m_statusbar = this->GetStatusBar();
	//wxRect rect;
	//m_statusbar->GetFieldRect(3, rect);
	//m_statusbar->Fit();

	//gauge = new wxGauge(m_statusbar, wxID_ANY, 100, rect.GetPosition(), rect.GetSize(), wxGA_SMOOTH);

	_camera = std::make_unique<SV::GS::OrotographicCamera>(_envelope.MinX * _zoomLevel, _envelope.MaxX * _zoomLevel, _envelope.MinY * _zoomLevel, _envelope.MaxY * _zoomLevel, -1.0f, 10.0f);
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

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTimeFPS);
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
	if (_canvas == nullptr || _isDataLoading || !this->IsActive()) return;

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

	QueryPerformanceCounter(&finalTimeFPS);
	double deltaTime = ((double)finalTimeFPS.QuadPart - (double)startTimeFPS.QuadPart) / (double)frequency.QuadPart;
	if (deltaTime >= 1)
	{
		SetStatusText(wxString::Format(wxT("FPS: %i"), (int)(frameCount / deltaTime)), 1);
		startTimeFPS = finalTimeFPS;
		frameCount = 0;
	}
	else {
		++frameCount;
	}
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

	const glm::vec3 afterZoom = ScreenToClipSpace(event.GetX(), event.GetY());

	_camera->SetPosition(_camera->GetPosition() + (beforeZoom - afterZoom));
}

void cMainWindow::OnSize(wxSizeEvent& event)
{
	wxFrame::OnSize(event);
	_canvas->Resize(event.GetSize());
}

void cMainWindow::OnLeftMouseBttDown(wxMouseEvent& event)
{
	wxSetCursor(wxCursor(wxCURSOR_HAND));
	_isDragged = true;
	_lastMousePosition = ScreenToClipSpace(event.GetX(), event.GetY());
}

void cMainWindow::OnLeftMouseBttUp(wxMouseEvent& event)
{
	if (_isDragged) {
		wxSetCursor(wxCursor(wxCURSOR_ARROW));
		_isDragged = false;
		//_lastMousePosition.x = 0;
		//_lastMousePosition.y = 0;
	}
}

void cMainWindow::OnRightMouseBttUp(wxMouseEvent& event)
{
	const glm::vec3 currentMousePosition = ScreenToClipSpace(event.GetX(), event.GetY());
	boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> pt(currentMousePosition.x, currentMousePosition.y);


	for (size_t i = 0; i < _layers.size(); ++i) {

		if (_layers[i]->IsVisible() && _layers[i]->IsPointInLayer(currentMousePosition)) {

			for (size_t j = 0; j < _layers[i]->Count(); ++j) {
				SV::Shapes::Polygon* yy =(SV::Shapes::Polygon*)(*_layers[i])[j];
			
				if (boost::geometry::within(pt,yy->GetBoostPolygon()) == false) continue;
				yy->SetSelected(!yy->IsSelected());
			}

		}
	}
}

void cMainWindow::OnMouseMove(wxMouseEvent& event)
{
	const glm::vec3 coordinates = ScreenToClipSpace(event.GetX(), event.GetY());
	SetStatusText(wxString::Format(wxT("%f %f"), coordinates.x, coordinates.y), 0);

	if (_isDragged == false) return;
	auto state = wxGetMouseState();
	if (state.LeftIsDown()) {

		wxSetCursor(wxCursor(wxCURSOR_HAND));

		long x = 0, y = 0;
		event.GetPosition(&x, &y);

		const glm::vec3 currentMousePosition = ScreenToClipSpace(x, y);
		const glm::vec3 delta = currentMousePosition - _lastMousePosition;

		_cameraPosition.x -= delta.x;
		_cameraPosition.y += delta.y;
		_camera->SetPosition(_cameraPosition);
		_lastMousePosition = currentMousePosition;
	}
}

void cMainWindow::OnScrollMouseDoubleClicked(wxMouseEvent& event)
{
	_zoomLevel = 1.0f;
	_camera->SetProjection(_envelope.MinX, _envelope.MaxX, _envelope.MinY, _envelope.MaxY);
	_cameraPosition = glm::vec3(0.0f);
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

		_isDataLoading = true;
		SetStatusText(wxT("Loading shp file"), 3);

		SV::EsriShpLayer* lay = new SV::EsriShpLayer(openFileDialog.GetFilename().c_str().AsChar(), openFileDialog.GetPath().c_str().AsChar(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));

		//if (lay->ReadShapeFile()) {
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
			_camera->SetProjection(left, right, bottom, top);
			_cameraPosition = glm::vec3(0.0f);
			_camera->SetPosition(_cameraPosition);

		}
		_layers.push_back(lay);
		_isDataLoading = false;
		//}
	}

	SetStatusText(wxT(""), 3);
	/*gauge->SetValue(0);*/
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

glm::vec3 cMainWindow::ScreenToClipSpace(float posX, float posY) const
{
	glm::vec3 projected(posX, (float)GetSize().GetHeight()- posY, 1.0f);
	glm::vec4 viewport(0, 0, (float)GetSize().GetWidth(), (float)GetSize().GetHeight());
	glm::vec3 coords = glm::unProject(projected, _camera->GetViewMatrix(), _camera->GetProjectionMatrix(), viewport);
	return coords;
}

glm::vec3 cMainWindow::ScreenToNdc(float posX, float posY) const
{
	const glm::vec3 ndcCoords(posX / (GetSize().GetWidth() * 0.5f) - 1, (GetSize().GetHeight() - posY) / (GetSize().GetHeight() * 0.5f) - 1, 0.0f);
	return ndcCoords;
}
