#include"stdafx.h"
#include "EsriShpLayer.h"
#include "Shapes/Polygon.h"
#include <algorithm>
#include "Render/DeviceContext.h"
#include "core/Log.h"
#include "Widgets/Shapes/EsriShpPolygon.h"

namespace SV::GS {

	EsriShpLayer::EsriShpLayer(const char* name, const char* path, glm::vec4 color, glm::vec4 outlineColor)
		:ILayer(name, color, outlineColor)
		, _shapePath(path)
	{

	}

	EsriShpLayer::~EsriShpLayer()
	{
		for (auto& s : _shapes) {
			if (s == nullptr) continue;
			delete s;
			s = nullptr;
		}
		_shapes.clear();

	}

	void EsriShpLayer::AddShape(const std::vector<glm::vec3>& vertices)
	{

	}

	bool EsriShpLayer::ReadShapeFile()
	{
		GDALAllRegister();
		poDS.reset(GDALDataset::Open(_shapePath.c_str(), GDAL_OF_VECTOR));
		if (poDS == nullptr)
		{
			CAD_ERROR("Open file {0} failed.", _name);
			return false;
		}

		for (OGRLayer* poLayer : poDS->GetLayers()) {

			for (const auto& poFeature : *poLayer)
			{
				const OGRGeometry* poGeometry = poFeature->GetGeometryRef();
				if (poGeometry != nullptr
					&& wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
				{
					_shapes.push_back(new EsriShpPolygon(this, (OGRPolygon*)poGeometry));
				}
				else if (poGeometry != NULL)
				{
					CAD_WARN("Geometry {0} is not implemented.", wkbFlatten(poGeometry->getGeometryType()));
				}

				else
				{
					CAD_ERROR("Invalid geometry");
				}
			}

			OGREnvelope env;
			poLayer->GetExtent(&env);

			_envelope.MaxX = std::max(env.MaxX, _envelope.MaxX);
			_envelope.MinX = std::min(env.MinX, _envelope.MinX);
			_envelope.MaxY = std::max(env.MaxY, _envelope.MaxY);
			_envelope.MinY = std::min(env.MinY, _envelope.MinY);
		}
		
		//auto  proj_string = poLayer->GetSpatialRef()->GetAttrValue("PROJCS", 0);
		////# geographic coordinate system
		//auto geog_string = poLayer->GetSpatialRef()->GetAttrValue("GEOGCS", 0);
		////# EPSG Code if available
		//auto epsg = poLayer->GetSpatialRef()->GetAttrValue("AUTHORITY", 1);
		////# datum 
		//auto datum = poLayer->GetSpatialRef()->GetAttrValue("DATUM", 0);
		//auto x = poLayer->GetSpatialRef();
		return true;
	}

}