#include "stdafx.h"
#include "LayerControl.h"

#include "Widgets/EsriShpLayer.h"
#include "Widgets/Envelope.h"
#include "Widgets/Shapes/Polygon.h"

namespace SV {

	wxDEFINE_EVENT(OnLayerAdded, wxCommandEvent);

	LayerControl::LayerControl(wxWindow* parent, wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		int nStrings,
		const wxString choices[],
		long style,
		const wxValidator& validator,
		const wxString& name)
		:wxCheckListBox(parent, id, pos, size, nStrings,choices, style, validator, name)
	{
		SetBackgroundColour(wxColor(200, 200, 200));
		Bind(wxEVT_CHECKLISTBOX, &LayerControl::OnCheckChanged, this);
	}

	LayerControl::~LayerControl()
	{
		for (auto& l : _layers) {
			delete l;
			l = nullptr;
		}
		_layers.clear();
	}

	void LayerControl::LoadEsriShapeFile(const std::filesystem::path& filePath)
	{
		SV::EsriShpLayer* lay = new SV::EsriShpLayer(filePath.filename().string().c_str(), filePath.string().c_str(),
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));

		_layers.push_back(lay);

		int id = Append(lay->GetName());
		Check(id, lay->IsVisible());

		wxCommandEvent myEvent(OnLayerAdded, GetId());
		myEvent.SetEventObject(this);
		ProcessWindowEvent(myEvent);
	}

	void LayerControl::LoadCgxmlFile(const std::filesystem::path& filePath)
	{
	}

	void LayerControl::LoadCgxmlFiles(const std::filesystem::path& folderPath)
	{
    }

    void LayerControl::LoadDummyTestPrimitives() {

		SV::Layer *lay = new SV::Layer("DummyLayer", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.2f, 0.0f, 0.0f, 0.8f));
        SV::Shapes::Polygon *Polygon = new SV::Shapes::Polygon(lay, "POLYGON ((-0.6495 -0.3750, 0.0000 0.7500, 0.6495 -0.3750))");
        lay->_shapes.push_back(Polygon);
        _layers.push_back(lay);
    }

	void LayerControl::OnCheckChanged(wxCommandEvent& ev)
	{
		int layerIndex = ev.GetInt();
		if (layerIndex < 0 || _layers.size() <= layerIndex) {
			ev.Skip();
		}
		else {
			_layers[layerIndex]->SetVisibility(!_layers[layerIndex]->IsVisible());
			ev.Skip();
		}

	}

}
