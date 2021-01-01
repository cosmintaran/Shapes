#include "stdafx.h"
#include "ILayer.h"

namespace SV::GS {

	void ILayer::Draw(DeviceContext& deviceContext)
	{
		if (!_isVisible) return;
		for (size_t i = 0; i < _shapes.size(); ++i) {
			_shapes[i]->vDraw(deviceContext);
		}
	}

	ILayer::~ILayer()
	{
		for (auto& s : _shapes) {
			if (s == nullptr) continue;
			delete s;
			s = nullptr;
		}
		_shapes.clear();
	}

	ILayer::ILayer(const char* name, glm::vec4 color, glm::vec4 outlineColor)
		:_name(name)
		,_color(color)
		,_outlineColor(outlineColor)
	{

	}

}
