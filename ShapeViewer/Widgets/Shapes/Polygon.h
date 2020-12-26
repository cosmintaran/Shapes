#pragma once
#include "IShape.h"
#include <vector>

namespace SV::GS {

	class Polygon : public IShape {
	private:

	public:
		Polygon();
		Polygon(const Layer* layer);
		Polygon(const Layer* layer,const std::vector<glm::vec3>& vertices);
		void Draw(DeviceContext& context) const override;
		void AddVertex(double x, double y, double z);
		void InsertVertexAt(uint32_t index,double x, double y, double z);
		bool IsOutlined()const { return _isOutlined;}
		void SetOutline(bool value) { _isOutlined = value; }

	private:
		void Triangulate();
		void CalculateBoundingBox();
	};

}