#pragma once
#include "ILayer.h"
#include <ogrsf_frmts.h>

namespace SV::GS {

	class DeviceContext;

	class EsriShpLayer : public ILayer {

	private:
		std::string _shapePath;
		GDALDatasetUniquePtr poDS;
		std::vector<std::future<void>> _futures;

	public:
		EsriShpLayer(const char* name, const char* path, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
		virtual ~EsriShpLayer();
		bool ReadShapeFile();
	private:
		
	};
}
