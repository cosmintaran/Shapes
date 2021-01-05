#pragma once
#include "../Drawable.h"
#include "Shape.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> point;
typedef boost::geometry::model::polygon<point,false, true> BoostPolygon;
class TPPLPoly;
namespace SV::Shapes {

	class Polygon :public Drawable, public Shape {
	private:
		Polygon() = delete;
	public:
		Polygon(Layer* layer);
		Polygon(Layer* layer, const char* wtk);
		void vDraw(GS::DeviceContext& deviceContext) override;
		[[nodiscard]] const Envelope& GetEnvelope()const override;
		[[nodiscard]] size_t Count() const override;
		[[nodiscard]] size_t InnerCount() const;
		void SetSelected(bool value)override { _isSelected = value; };
		const BoostPolygon& GetBoostPolygon()const { return _polygon; }

	protected:
		void PrepareToDraw() override;
		void PolyPart_Triangulate(std::list<TPPLPoly>& polyList);
		void Triangulate();


	protected:
		const char* _wktPolygon;
		bool _isTriangulated = false;
		bool _isOutlined = false;
		Envelope _envelope;
		BoostPolygon _polygon;
	};

}
