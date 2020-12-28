#pragma once
#include "ILayer.h"
#include <ogrsf_frmts.h>

namespace SV::GS {

	class DeviceContext;

	class EsriShpLayer : public ILayer {

	private:
		std::string _shapePath;
		GDALDatasetUniquePtr poDS;

	public:
		EsriShpLayer(const char* name, const char* path, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
		virtual ~EsriShpLayer();
		void AddShape(const std::vector<glm::vec3>& vertices);
		bool ReadShapeFile();

	private:

	};
}