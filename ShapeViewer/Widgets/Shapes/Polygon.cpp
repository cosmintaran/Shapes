#include"stdafx.h"
#include "Polygon.h"
#include <algorithm>
#include "Widgets/Shapes/Polygon.h"
#include <poly2tri.h>
#include <algorithm>
#include "Widgets/ILayer.h"

namespace SV::GS {

	Polygon::Polygon(const ILayer* layer)
		:IShape(layer)
	{

	}

	//Polygon::Polygon(const ILayer* layer,const std::vector<glm::vec3>& vertices)
	//	:IShape(layer)
	//{
	//	if (layer == nullptr) throw "Null layer is not allowed!";

	//	for (auto& vert : vertices)
	//	{
	//		Vertex v;
	//		v.Position = vert;
	//		v.Color = layer ? layer->GetColor() : glm::vec4(1.0f);
	//		_vertices.emplace_back(std::move(v));
	//	}
	//	Triangulate();
	//	CalculateBoundingBox();
	//}

	void Polygon::Triangulate()
	{
		std::vector<p2t::Point*> polyline;

		for (size_t i = 0; i < _vertices.size() - 1; ++i) {
			auto pt = new p2t::Point(_vertices[i].Position.x, _vertices[i].Position.y);
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
	
	//void Polygon::CalculateBoundingBox()
	//{
	//	std::for_each(_vertices.cbegin(), _vertices.cend(), [&](const Vertex& vertex)
	//		{
	//			_boundingBox.MaxX = std::max((double)vertex.Position.x, _boundingBox.MaxX);
	//			_boundingBox.MaxY = std::max((double)vertex.Position.y, _boundingBox.MaxY);

	//			_boundingBox.MinX = std::min((double)vertex.Position.x, _boundingBox.MinX);
	//			_boundingBox.MinY = std::min((double)vertex.Position.y, _boundingBox.MinY);
	//		});
	//}
}
