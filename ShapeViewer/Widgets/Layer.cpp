#include "stdafx.h"
#include "Layer.h"
#include "Widgets/Drawable.h"
namespace SV {

	void Layer::Draw(GS::DeviceContext& deviceContext)
	{
		if (!_isVisible) return;
		for (size_t i = 0; i < _shapes.size(); ++i) {
			_shapes[i]->vDraw(deviceContext);
		}
	}

	bool Layer::IsPointInLayer(const glm::vec3& point) const
	{
		return _envelope.MinX <= point.x && _envelope.MaxX >= point.x &&
			   _envelope.MinY <= point.y && _envelope.MaxY >= point.y;
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

	Layer::Layer(const char* name, glm::vec4 color, glm::vec4 outlineColor)
		:_name(name)
		,_color(color)
		,_outlineColor(outlineColor)
	{

	}

}
