#pragma once
#include "Layer.h"

#include <ogrsf_frmts.h>

namespace SV {

	class EsriShpLayer : public Layer {

	private:
		std::string _shapePath;
		GDALDatasetUniquePtr poDS;
		std::vector<std::future<void>> _futures;

	public:
		EsriShpLayer(const char* name, const char* path, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
		virtual ~EsriShpLayer();
		
	private:
		bool ReadShapeFile();
		void ReadShapeFileAsync();
	};
}
