#include "stdafx.h"
#include "EsriShpPolygon.h"
#include "../EsriShpLayer.h"
#include "core/Log.h"

namespace SV::GS
{

	EsriShpPolygon::EsriShpPolygon(const ILayer* layer, OGRPolygon polygon)
		:Polygon(layer)
		, _ogrPolygon(polygon)
	{
		
	}


	EsriShpPolygon::~EsriShpPolygon()	{

	}

	void EsriShpPolygon::Read()
	{
		int NumberOfInnerRings = _ogrPolygon.getNumInteriorRings();
		OGRLinearRing* poExteriorRing = _ogrPolygon.getExteriorRing();
		int NumberOfExteriorRingVertices = poExteriorRing->getNumPoints();
		_vertices.reserve(NumberOfExteriorRingVertices);
		OGRPoint ptTemp;
		for (int k = 0; k < NumberOfExteriorRingVertices; k++)
		{
			poExteriorRing->getPoint(k, &ptTemp);

			////OGRCoordinateTransformation* coordTrans =
			////	OGRCreateCoordinateTransformation(ptTemp.getSpatialReference(), &);

			////coordTrans->Transform();
			Vertex v;
			v.Position = glm::vec3(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ());
			v.Color = _layer ? _layer->GetColor() : glm::vec4(1.0f);
			_vertices.emplace_back(std::move(v));
		}
        PolyPart_Triangulate();
	}
}
