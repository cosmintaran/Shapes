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
}
