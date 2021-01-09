#include "stdafx.h"
#include "Polygon.h"
#include "Widgets/Layer.h"
#include "Widgets/Shapes/Polygon.h"
#include <poly2tri.h>

#include "core/Log.h"
#include <polypartition.h>
#include <iostream>

#include "boost/polygon/voronoi.hpp"


using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;

struct Point
{
    double a;
    double b;
    Point(double x, double y) : a(x), b(y) {}
};


namespace SV::Shapes {

    Polygon::Polygon(Layer* layer)
        :Drawable(layer), _isOutlined(true)
    {
        PrepareToDraw();
    }

    Polygon::Polygon(Layer* layer, const char* wtk)
        : Drawable(layer),_isOutlined(true)
    {
        boost::geometry::read_wkt(wtk, _polygon);
        PrepareToDraw();

    }

    void Polygon::vDraw(GS::DrawingContext& deviceContext)
    {
        if (!_isTriangulated) return;

        if (!_isGraphicsEnabled) {
            GraphicsInitialization();
        }
        if (!_isSelected) {
            deviceContext.SetDrawColor(_layer->GetR(), _layer->GetG(), _layer->GetB(), _layer->GetA());
        }
        else {
            deviceContext.SetDrawColor(0.03921568627f, 0.6313725490196f, 0.9490196078431373f, 1.0f);
        }
        glBindVertexArray(_vertexArrayRenderID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexTrianglesBufferRenderID);
        glDrawElements(GL_TRIANGLES, indeciesContainer.IndexTrianglesBuffer.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        if (!_isOutlined) return;

        deviceContext.SetDrawColor(_layer->GetOutlineColor().r, _layer->GetOutlineColor().g, _layer->GetOutlineColor().b, _layer->GetOutlineColor().a);
        glBindVertexArray(_vertexArrayRenderID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexLinesBufferRenderID);
        glDrawElements(GL_LINE_LOOP, indeciesContainer.IndexLinesBuffer.size(), GL_UNSIGNED_INT, /*(void*) bufferSize*/ nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    const Envelope& Polygon::GetEnvelope() const
    {
        return _envelope;
    }

    size_t Polygon::Count() const
    {
        return boost::geometry::num_points(_polygon);
    }

    size_t Polygon::InnerCount() const
    {
        return 0;
    }

    void Polygon::PrepareToDraw()
    {
       auto ringType= _polygon.outer();
       _vertices.reserve(ringType.size());

       TPPLPoly poly;
       std::vector<Point>points;
       points.reserve(ringType.size());
       poly.Init(ringType.size());
       std::list<TPPLPoly> polyList;

       size_t k = 0;
       for (auto it = boost::begin(boost::geometry::exterior_ring(_polygon)); it != boost::end(boost::geometry::exterior_ring(_polygon)); ++it)
       {
           const float x = boost::geometry::get<0>(*it);
           const float y = boost::geometry::get<1>(*it);

           SV::GS::Vertex v;
           v.Position = glm::vec3(x, y, 0);
           _vertices.emplace_back(std::move(v));
           indeciesContainer.IndexLinesBuffer.push_back(k);

           poly[k].x = x;
           poly[k].y = y;
           poly[k].id = k;
           points.emplace_back(x, y);
           ++k;
       }
       poly.SetOrientation(TPPL_CCW);
       polyList.push_back(poly);

       //check for holes
        if (_polygon.inners().size() > 0) {
            
            for (auto& r : _polygon.inners())
            {
                TPPLPoly innerPoly;
                innerPoly.Init(r.size());
                innerPoly.SetHole(true);
                size_t j = 0;

                for (auto& p : r) {

                    const float x = boost::geometry::get<0>(p);
                    const float y = boost::geometry::get<1>(p);

                    SV::GS::Vertex v;
                    v.Position = glm::vec3(x, y, 0);
                    _vertices.emplace_back(std::move(v));
                    indeciesContainer.IndexLinesBuffer.push_back(k);

                    innerPoly[j].x = x;
                    innerPoly[j].y = y;
                    innerPoly[j].id = k;
                    ++k;
                    ++j;
                }
                innerPoly.SetOrientation(TPPL_CW);
                polyList.push_back(innerPoly);
            }
        }

        PolyPart_Triangulate(polyList);
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

        indeciesContainer.IndexTrianglesBuffer.reserve(rez.size() * 3);
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
                indeciesContainer.IndexTrianglesBuffer.push_back(static_cast<unsigned int>(polyline.size() - (polyline.cend() - findIt)));
            }
            });

        delete cdt;
        for (int i = 0; i < polyline.size(); ++i) {
            delete polyline[i];
        }
        _isTriangulated = true;
    }

    void Polygon::PolyPart_Triangulate(std::list<TPPLPoly>& polyList) {

        TPPLPartition pp;   
        std::list<TPPLPoly>result;            
        
        pp.Triangulate_EC(&polyList, &result);

        for (auto p : result) {
            const int noOfPoints = p.GetNumPoints();
            if (noOfPoints != 3) continue; // not a triangle

            for (int i = 0; i < noOfPoints; ++i) {
                indeciesContainer.IndexTrianglesBuffer.push_back(static_cast<unsigned int>(p.GetPoint(i).id));
            }
        }
        _isTriangulated = true;
    }
}
