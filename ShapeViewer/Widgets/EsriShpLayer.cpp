#include"stdafx.h"
#include "EsriShpLayer.h"
#include "Shapes/Polygon.h"
#include "Render/DeviceContext.h"
#include "core/Log.h"
#include "Widgets/Shapes/Polygon.h"
#include <future>

namespace SV {

	static std::mutex _mutex;

	EsriShpLayer::EsriShpLayer(const char* name, const char* path, glm::vec4 color, glm::vec4 outlineColor)
		:Layer(name, color, outlineColor)
		, _shapePath(path)
	{
		ReadShapeFile();
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


	bool EsriShpLayer::ReadShapeFile()
	{
		GDALAllRegister();
		poDS.reset(GDALDataset::Open(_shapePath.c_str(), GDAL_OF_VECTOR));
		if (poDS == nullptr)
		{
			CAD_ERROR("Open file {0} failed.", _name);
			return false;
		}

		OGRGeometry* poGeometry = nullptr;

		for (OGRLayer* poLayer : poDS->GetLayers()) {

			for (const auto& poFeature : *poLayer)
			{
				poGeometry = poFeature->GetGeometryRef();
				if (poGeometry != nullptr
					&& (wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon /*|| wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPolygon*/))
				{
					char* wkt;
					poGeometry->exportToWkt(&wkt);
					_futures.emplace_back(std::async(std::launch::async, [&, wkt]() {
						Shapes::Polygon* shp = new Shapes::Polygon(this, wkt);
						std::lock_guard<std::mutex> locking(_mutex);
						_shapes.push_back(shp);
						}));

					/*delete[]pgeos;*/
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

		return true;
	}
}
