#include "stdafx.h"
#include "Polygon.h"
#include "Widgets/ILayer.h"
#include "Widgets/Shapes/Polygon.h"
#include <poly2tri.h>

#include "core/Log.h"
#include <polypartition.h>
#include <iostream>

namespace SV::GS {

	Polygon::Polygon(const ILayer* layer)
		:IShape(layer)
	{

	}

	void Polygon::Triangulate() {
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

			for (size_t i = 0; i < triangle.size(); i++) {
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

	void Polygon::PolyPart_Triangulate() {

		TPPLPartition pp;
		TPPLPoly poly;
		std::list<TPPLPoly> polyList, result;

		poly.Init(_vertices.size());
		for (size_t i = 0; i < _vertices.size(); ++i) {
		    poly[i].x = _vertices[i].Position.x;
		    poly[i].y = _vertices[i].Position.y;
			poly[i].id = i;
		}
		poly.SetOrientation(TPPL_CCW);
		polyList.push_back(poly);

		pp.Triangulate_EC(&polyList, &result);

		for (auto p : result) {
			const int noOfPoints = p.GetNumPoints();
			if (noOfPoints > 3) continue; // not a triangle

			for (int i = 0; i < noOfPoints; ++i) {
				_indecies.push_back(static_cast<unsigned int>(p.GetPoint(i).id));
			}
		}
	
	}
} // namespace SV::GS
