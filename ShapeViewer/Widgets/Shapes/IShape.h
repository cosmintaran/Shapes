#pragma once
#include<vector>
#include<glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <float.h>

#include "Widgets/Shapes/Triangle.h"
#include "Render/DeviceContext.h"


namespace SV::GS {
	class Layer;

	struct BBox {
		BBox() {
			xMax = DBL_MIN;
			xMin = DBL_MAX;
			yMax = DBL_MIN;
			yMin = DBL_MAX;
		}
		double xMax, xMin, yMax, yMin;
	};

	class IShape {

	public:
		virtual ~IShape() = default;
		virtual void Draw(DeviceContext& context) const = 0;
		[[nodiscard]] size_t Count() const{ return _vertices.size(); }
		[[nodiscard]] const BBox& GetBoundingBox()const { return _boundingBox; }

	protected:

		IShape(const Layer* layer);
		IShape(const Layer* layer, const std::vector<glm::vec3>& vertices);

		bool _isSelected = false;
		bool _isOutlined = true;
		BBox _boundingBox;
		const Layer* _layer = nullptr;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indecies;
	};
}