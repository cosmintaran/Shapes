#include "stdafx.h"
#include "EsriShpPolygon.h"
#include "../EsriShpLayer.h"
#include "core/Log.h"
#include <glm/gtc/type_ptr.hpp>

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
			Vertex v;
			v.Position = glm::vec3(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ());
			/*v.Color = _layer ? _layer->GetColor() : glm::vec4(1.0f);*/
			_vertices.emplace_back(std::move(v));
			indeciesContainer.IndexLinesBuffer.push_back(k);
		}
        PolyPart_Triangulate();
	}

	void EsriShpPolygon::vDraw(DeviceContext& deviceContext) const
	{
		deviceContext.SetDrawColor(_layer->GetR(), _layer->GetG(), _layer->GetB(), _layer->GetA());
		glBindVertexArray(_vertexArrayRenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexTrianglesBufferRenderID);
		glDrawElements(GL_TRIANGLES, indeciesContainer.IndexTrianglesBuffer.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (!_isOutlined) return;

		deviceContext.SetDrawColor( _layer->GetOutlineColor().r, _layer->GetOutlineColor().g, _layer->GetOutlineColor().b, _layer->GetOutlineColor().a);
		glBindVertexArray(_vertexArrayRenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexLinesBufferRenderID);
		glDrawElements(GL_LINE_LOOP, indeciesContainer.IndexLinesBuffer.size(), GL_UNSIGNED_INT, /*(void*) bufferSize*/ nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
