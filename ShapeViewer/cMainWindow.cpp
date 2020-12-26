#include "stdafx.h"
#include "cMainWindow.h"
#include "core/Id.h"
#include "Widgets/Shapes/ShpTypes.h"
#include "core/Log.h"
#include <ogrsf_frmts.h>
#include <filesystem>
#include <stdlib.h>



cMainWindow::cMainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name), _aspectRatio(0.0f)
{


	InitializeUIComponents(size);

	//Test 
	SV::GS::Layer* layer1 = new SV::GS::Layer("TestLayer", SV::GS::ShapeType::Polygon, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));
	_layers.emplace_back(layer1);

	for (int i = 0; i < 50; ++i) {
		std::vector<glm::vec3> polygon;
		polygon.emplace_back(((float)rand() / (RAND_MAX)) + 1.5, ((float)rand() / (RAND_MAX)) + 1, 0.0f);
		polygon.emplace_back(((float)rand() / (RAND_MAX)) + 1.5, ((float)rand() / (RAND_MAX)) + 1, 0.0f);
		polygon.emplace_back(((float)rand() / (RAND_MAX)) + 1.5, ((float)rand() / (RAND_MAX)) + 1, 0.0f);
		_layers[0]->AddShape(polygon);
	}

	//std::vector<glm::vec3> polygon1;
	//polygon1.emplace_back(-0.3812f, 0.3490f, 0.0f);
	//polygon1.emplace_back(-0.0197f, 0.1910f, 0.0f);
	//polygon1.emplace_back(0.3598f, 0.1910f, 0.0f);
	//polygon1.emplace_back(0.4728f, 0.5342f, 0.0f);
	//polygon1.emplace_back(-0.2366f, 0.6787f, 0.0f);
	//_layers[0]->AddShape(polygon1);

	//std::vector<glm::vec3> polygon2;
	//polygon2.emplace_back(-0.3812f, 0.3490f, 0.0f);
	//polygon2.emplace_back(-0.0197f, 0.1910f, 0.0f);
	//polygon2.emplace_back(0.3598f, 0.1910f, 0.0f);
	//polygon2.emplace_back(0.4638f, 0.0600f, 0.0f);
	//polygon2.emplace_back(0.4050f, -0.3238f, 0.0f);
	//polygon2.emplace_back(0.0842f, -0.2470f, 0.0f);
	//polygon2.emplace_back(0.0842f, -0.1206f, 0.0f);
	//polygon2.emplace_back(-0.3540f, -0.1206f, 0.0f);
	//polygon2.emplace_back(-0.4399f, 0.0420f, 0.0f);
	//_layers[0]->AddShape(polygon2);

	//std::vector<glm::vec3> polygon3;
	//polygon3.emplace_back(-0.5980f, 0.1007f, 0.0f);
	//polygon3.emplace_back(-0.2411f, 0.1007f, 0.0f);
	//polygon3.emplace_back(-0.1101f, -0.2877f, 0.0f);
	//polygon3.emplace_back(-0.1778f, -0.5360f, 0.0f);
	//polygon3.emplace_back(-0.3450f, -0.5360f, 0.0f);
	//polygon3.emplace_back(-0.6568f, -0.2877f, 0.0f);
	//polygon3.emplace_back(-0.7457f, -0.2169f, 0.0f);
	//_layers[0]->AddShape(polygon3);

	//SV::GS::Layer* layer2 = new SV::GS::Layer("TestLayer2", SV::GS::ShapeType::Polygon, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//_layers.emplace_back(layer2);
	//std::vector<glm::vec3> polygon4;
	//polygon4.emplace_back(0.2413f, -0.6381f, 0.0f);
	//polygon4.emplace_back(0.5021f, -0.4861f, 0.0f);
	//polygon4.emplace_back(0.6827f, -0.6381f, 0.0f);
	//polygon4.emplace_back(0.3760f, -0.8359f, 0.0f);
	//_layers[1]->AddShape(polygon4);

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

	CreateStatusBar(2);
	SetStatusText(wxT("0.00000 0.00000 "), 0);

	_aspectRatio = (float)size.GetWidth() / (float)size.GetHeight();
	_camera = std::make_unique<SV::GS::OrotographicCamera>((-_aspectRatio * _zoomLevel), (_aspectRatio * _zoomLevel), -_zoomLevel, _zoomLevel, -1.0f, 1.0f);
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
	//CAD_TRACE("{0}.s", ts);

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
	if (zoomLevel < 0.01 || zoomLevel > 10) return;

	const glm::vec4 beforeZoom = ScreenToClipSpace(event.GetX(), event.GetY());
	_zoomLevel = zoomLevel;
	_camera->SetProjection((-_aspectRatio * _zoomLevel), (_aspectRatio * _zoomLevel), -_zoomLevel, _zoomLevel);
	const glm::vec4 afterZoom = ScreenToClipSpace(event.GetX(), event.GetY());

	const glm::vec4 diff = beforeZoom - afterZoom;
	_camera->SetPosition(_camera->GetPosition() + glm::vec3(diff.x, diff.y, 0.0f));
}

void cMainWindow::OnSize(wxSizeEvent& event)
{
	wxFrame::OnSize(event);
	_aspectRatio = (float)event.GetSize().GetWidth() / (float)event.GetSize().GetHeight();
	_canvas->Resize(event.GetSize());
	_camera->SetProjection((-_aspectRatio * _zoomLevel), (_aspectRatio * _zoomLevel), -_zoomLevel, _zoomLevel);

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
	_camera->SetProjection((-_aspectRatio * _zoomLevel), (_aspectRatio * _zoomLevel), -_zoomLevel, _zoomLevel);
	_cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
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

	//Sterge memoria GDAL!!!!!!!!!!!!!!!!!!!!

	if (openFileDialog.ShowModal() == wxID_OK) {
		GDALAllRegister();

		GDALDataset* poDS = nullptr;
		poDS = (GDALDataset*)GDALOpenEx(openFileDialog.GetPath(), GDAL_OF_VECTOR, NULL, NULL, NULL);
		if (poDS == nullptr)
		{
			CAD_ERROR("Open file {0} failed.", openFileDialog.GetFilename());
			return;
		}

		OGRLayer* poLayer;

		wxString fileName = openFileDialog.GetFilename().RemoveLast(4);
		size_t pos = fileName.find(".shp");
		if (pos > 0) {

		}
		poLayer = poDS->GetLayerByName(fileName);
		if (poLayer == nullptr) {

			CAD_ERROR("Open file {0} failed.", openFileDialog.GetFilename());
			return;
		}
		for (auto& poFeature : poLayer)
		{
			OGRFeature* poFeature;

			poLayer->ResetReading();
			while ((poFeature = poLayer->GetNextFeature()) != NULL)
			{
				for (auto&& oField : *poFeature)
				{
					switch (oField.GetType())
					{
					case OFTInteger:
						CAD_INFO("{0}", oField.GetInteger());
						break;
					case OFTInteger64:
						CAD_INFO("{0}", oField.GetInteger64());
						break;
					case OFTReal:
						CAD_INFO("{0}", oField.GetDouble());
						break;
					case OFTString: {
						bool isEmpty = oField.empty();
						if (!isEmpty) {
							CAD_INFO(oField.GetAsString());
						}
					}
						break;
					default:
						CAD_INFO(oField.GetAsString());
						break;
					}
				}
			}
			OGRFeature::DestroyFeature(poFeature);
		}

		GDALClose(poDS);
	}

}

glm::vec4 cMainWindow::ScreenToClipSpace(float posX, float posY) const
{
	const glm::vec4 ndcCoords = ScreenToNdc(posX, posY);
	const glm::vec4 translatedNDC(_camera->GetPosition().x, _camera->GetPosition().y, 0.0f, 0.0f);
	const glm::vec4 coords = _camera->GetInvercedViewProjection() * ndcCoords;
	return coords + translatedNDC;
}

glm::vec4 cMainWindow::ScreenToNdc(float posX, float posY) const
{
	const glm::vec4 ndcCoords(posX / (GetSize().GetWidth() * 0.5f) - 1, (GetSize().GetHeight() - posY) / (GetSize().GetHeight() * 0.5f) - 1, 0.0f, 0.0f);
	return ndcCoords;
}
