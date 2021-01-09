#include "stdafx.h"
#include "cMainWindow.h"
#include "core/Id.h"
#include "Widgets/Shapes/ShpTypes.h"
#include <wx/progdlg.h>

#include <wx/splitter.h>


BEGIN_EVENT_TABLE(cMainWindow, wxFrame)
//EVT_CLOSE(cMainWindow::OnClose)
	//EVT_COMMAND(MapControl::GetId(),MapControl::OnMapMouseMove, &cMainWindow::OnCoordsUpdatedCmd)
END_EVENT_TABLE()


cMainWindow::cMainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name)
{
	InitializeUIComponents(size);
}

void cMainWindow::InitializeUIComponents(const wxSize& size)
{
	wxMenuBar* m_pMenuBar = new wxMenuBar();

	// File Menu
	wxMenu* m_pFileMenu = new wxMenu();
	m_pFileMenu->Append(wxID_OPEN, _T("&Open"));
	m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
	m_pFileMenu->AppendSeparator();
	m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
	m_pMenuBar->Append(m_pFileMenu, _T("&File"));
	// About menu
	wxMenu* m_pHelpMenu = new wxMenu();
	m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
	m_pMenuBar->Append(m_pHelpMenu, _T("&Help"));

	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(cMainWindow::OnMenuOpenCmd));
	
	SetMenuBar(m_pMenuBar);
	CreateStatusBar(4);
	SetStatusText(wxT("0.00000 0.00000 "), 0);
	SetStatusText(wxT("                "), 2);

	wxSplitterWindow* spliter = new wxSplitterWindow(this, wxID_ANY, {0,0}, size, wxSP_BORDER | wxSP_LIVE_UPDATE);
	_map = new SV::MapControl(spliter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	_layerCtrl = new SV::LayerControl(spliter, wxID_ANY, { 0,0 });
	_map->AttachLayerControl(_layerCtrl);
	Bind(SV::OnMapMouseMove, &cMainWindow::OnCoordsUpdatedCmd, this, _map->GetId());
	Bind(SV::OnLayerAdded,   &SV::MapControl::OnLayerAddedCmd, _map, _layerCtrl->GetId());

	spliter->SetMinimumPaneSize(100);
	spliter->SplitVertically(_layerCtrl, _map, 250);

	Fit();
	Centre();

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTimeFPS);
}

cMainWindow::~cMainWindow()
{

}

void cMainWindow::vPaint(const SV::CORE::Timestep& ts)
{
	if (_map == nullptr || _isDataLoading || !this->IsActive()) return;
	_map->Paint(ts);

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



void cMainWindow::OnMenuOpenCmd(wxCommandEvent& WXUNUSED)
{
	wxFileDialog openFileDialog(this, "Select a shapefile", "", "", "shp files (*.shp)|*.shp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK) {
		_isDataLoading = true;
		SetStatusText(wxT("Loading shp file"), 3);
		_layerCtrl->LoadEsriShapeFile(std::filesystem::path(openFileDialog.GetPath().t_str()));	
		_isDataLoading = false;
	}

	SetStatusText(wxT(""), 3);
	/*gauge->SetValue(0);*/
	
}

void cMainWindow::OnCoordsUpdatedCmd(wxCommandEvent& ev)
{
	SetStatusText(ev.GetString(), 0);
}

