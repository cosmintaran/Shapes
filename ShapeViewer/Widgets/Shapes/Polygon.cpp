#include"stdafx.h"
#include "Polygon.h"
#include <algorithm>
#include "Widgets/Shapes/Polygon.h"
#include "Widgets/Layer.h"
#include <poly2tri.h>
#include <unordered_map>
#include <algorithm>

namespace SV::GS {
	Polygon::Polygon()
		:IShape(nullptr)
	{
	}

	Polygon::Polygon(const Layer* layer)
		:IShape(layer)
	{

	}

	Polygon::Polygon(const Layer* layer,const std::vector<glm::vec3>& vertices)
		:IShape(layer,vertices)
	{
		if (layer == nullptr) throw "Null layer is not allowed!";

		Triangulate();
		CalculateBoundingBox();
	}

	void Polygon::Draw(DeviceContext& context)const
	{
		context.DrawPolygon(_vertices, _indecies);

		if(_isOutlined)
			context.DrawLine(_vertices, _layer->GetOutlineColor());
	}

	void Polygon::AddVertex(double x, double y, double z)
	{
		Vertex v;
		v.Position = glm::vec3(x, y, z);
		v.Color = _layer? _layer->GetColor() : glm::vec4(1.0f);
		//v.TextCoords = glm::vec2(0.0f, 0.0f);
		//v.TextId = 0.0f;
		_vertices.emplace_back(std::move(v));
		CalculateBoundingBox();
	}

	void Polygon::InsertVertexAt(uint32_t index, double x, double y, double z)
	{
		if (index > _vertices.size())
			AddVertex(x, y, z);

		Vertex v;
		v.Position = glm::vec3(x, y, z);
		v.Color = _layer ? _layer->GetColor() : glm::vec4(1.0f);
		//v.TextCoords = glm::vec2(0.0f, 0.0f);
		//v.TextId = 0.0f;
		_vertices.insert(_vertices.begin()+index,std::move(v));

		CalculateBoundingBox();
	}

	void Polygon::Triangulate()
	{
		std::vector<p2t::Point*> polyline;

		for (auto& p : _vertices) {
			auto pt = new p2t::Point(p.Position.x, p.Position.y);
			polyline.push_back(pt);
		}

		p2t::CDT* cdt = new p2t::CDT(polyline);
		cdt->Triangulate();
		auto rez = cdt->GetTriangles();

		_indecies.reserve(rez.size() * 3);
		std::for_each(rez.cbegin(), rez.cend(), [&](p2t::Triangle* trig) {

			std::array<p2t::Point*, 3> triangle{
				trig->GetPoint(0),
				trig->GetPoint(1),
				trig->GetPoint(2)
			};
			
			unsigned int i = 0;

			for (size_t i = 0; i < triangle.size(); i++)
			{
				auto findIt = std::find(polyline.cbegin(), polyline.cend(), triangle[i]);
				if (findIt == polyline.cend())
					throw "Triangulation failed";
				_indecies.push_back(static_cast<unsigned int>(polyline.size() - (polyline.cend() - findIt)));
			}

			});

		delete cdt;
		for (int i = 0; i < polyline.size(); ++i) {
			delete polyline[i];
		}
	}
	
	void Polygon::CalculateBoundingBox()
	{
		std::for_each(_vertices.cbegin(), _vertices.cend(), [&](const Vertex& vertex)
			{
				_boundingBox.xMax = std::max((double)vertex.Position.x, _boundingBox.xMax);
				_boundingBox.yMax = std::max((double)vertex.Position.y, _boundingBox.yMax);

				_boundingBox.xMin = std::min((double)vertex.Position.x, _boundingBox.xMin);
				_boundingBox.yMin = std::min((double)vertex.Position.y, _boundingBox.yMin);
			});
	}
}
