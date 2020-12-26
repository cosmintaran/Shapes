#include"stdafx.h"
#include "Layer.h"
#include "Shapes/Polygon.h"
#include <algorithm>
#include "Render/DeviceContext.h"

namespace SV::GS {

	Layer::Layer(const std::string& name, ShapeType type, glm::vec4 color, glm::vec4 outlineColor)
		:_layerName(name)
		, _shapeType(type)
		, _color(color)
		, _outlineColor(outlineColor)
		, _isVisible(true)
		, _isDirty(true)
	{

	}

	Layer::~Layer()
	{
		for (auto& s : _shapes) {
			if (s == nullptr) continue;
			delete s;
			s = nullptr;
		}
		_shapes.clear();
	}

	void Layer::AddShape(const std::vector<glm::vec3>& vertices)
	{
		switch (_shapeType)
		{
		case GS::ShapeType::Null:
			break;
		case GS::ShapeType::Point:
			break;
		case GS::ShapeType::Polyline:
			break;
		case GS::ShapeType::Polygon: 
		{
			IShape* polygon = new Polygon(this, vertices);
			if(_shapes.size() == 0)
				_shapes.push_back(polygon);
			else {
				ShapeSortByMaxX pred;
				insert_sorted(_shapes, polygon, pred);
			}
			
		}
			break;
		case GS::ShapeType::MultiPoint:
			break;
		case GS::ShapeType::PointZ:
			break;
		case GS::ShapeType::PolylineZ:
			break;
		case GS::ShapeType::PolygonZ:
			break;
		case GS::ShapeType::MultiPointZ:
			break;
		case GS::ShapeType::PointM:
			break;
		case GS::ShapeType::PolylineM:
			break;
		case GS::ShapeType::PolygonM:
			break;
		case GS::ShapeType::MultiPointM:
			break;
		case GS::ShapeType::MultiPatch:
			break;
		default:
			break;
		}
	}

	void Layer::Draw(DeviceContext& context)
	{
		if (!_isVisible) return;

		for (size_t i = 0; i < _shapes.size(); ++i) {
			_shapes[i]->Draw(context);
		}
	}

}