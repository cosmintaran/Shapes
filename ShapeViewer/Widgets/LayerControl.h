#pragma once
#include <wx/checklst.h>
#include <wx/imaglist.h>
#include "Layer.h"

namespace SV {

	wxDECLARE_EVENT(OnLayerAdded, wxCommandEvent);

	class LayerControl : public
		wxCheckListBox {

	public:

		LayerControl(wxWindow* parent, wxWindowID id,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			int nStrings = 0,
			const wxString choices[] = NULL,
			long style = 0,
			const wxValidator& validator = wxDefaultValidator,
			const wxString& name = wxASCII_STR(wxListBoxNameStr));
		~LayerControl();

		const std::vector<SV::Layer*>& GetLayers() const{ return _layers; }

		void LoadEsriShapeFile(const std::filesystem::path& filePath);
		void LoadCgxmlFile(const std::filesystem::path& filePath);
		void LoadCgxmlFiles(const std::filesystem::path& folderPath);

	private:
		std::vector<SV::Layer*> _layers;

	};

}
