#pragma once
#include<vector>
#include<glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Widgets/Shapes/Triangle.h"
#include "Render/DeviceContext.h"
#include "Widgets/Envelope.h"

namespace SV::GS {
	class ILayer;

	class IShape {

	protected:
		bool _isSelected = false;
		bool _isOutlined = true;
		Envelope _envelope;
		const ILayer* _layer = nullptr;
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indecies;


	public:
		virtual ~IShape() = default;
		virtual void Draw(DeviceContext& context) const;
		[[nodiscard]] size_t Count() const{ return _vertices.size(); }
		[[nodiscard]] virtual const Envelope& GetEnvelope()const { return _envelope; };
		void SetOutline(bool value) { _isOutlined = value; }
		void SetSelected(bool value) { _isSelected = value; }
		bool IsOutlined()const { return _isOutlined; }
		virtual void Read(){ }
	protected:
		IShape(const ILayer* layer);		
	};
}
