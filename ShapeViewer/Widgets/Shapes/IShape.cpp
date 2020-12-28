#include"stdafx.h"
#include "IShape.h"
#include <Render/Vertex.h>
#include "../ILayer.h"
namespace SV::GS {

	IShape::IShape(const ILayer* layer)
		:_isSelected(false)
		, _layer(layer)
	{
	}

	void IShape::Draw(DeviceContext& context) const
	{
		context.DrawPolygon(_vertices, _indecies);

		if (_isOutlined)
			context.DrawLine(_vertices, _layer->GetOutlineColor());
	}

}
