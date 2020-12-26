#include"stdafx.h"
#include "IShape.h"
#include "../Layer.h"
#include <Render/Vertex.h>
namespace SV::GS {

	IShape::IShape(const Layer* layer)
		:_isSelected(false)
		, _layer(layer)
	{
	}

	IShape::IShape(const Layer* layer, const std::vector<glm::vec3>& vertices)
		: _isSelected(false)
		, _layer(layer)
	{
		for (auto& vert : vertices)
		{
			Vertex v;
			v.Position = vert;
			v.Color = layer? layer->GetColor() : glm::vec4(1.0f);
			_vertices.emplace_back(std::move(v));
		}
	}

}
