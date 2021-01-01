#pragma once
#include<vector>
#include<glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Widgets/Shapes/Triangle.h"
#include "Render/DeviceContext.h"
#include "Widgets/Envelope.h"
#include"Render/IRenderShape.h"

namespace SV::GS {
	class ILayer;

	class IShape: public IRenderShape<Vertex> {

	protected:
		bool _isSelected = false;
		bool _isOutlined = true;
		Envelope _envelope;
		const ILayer* _layer = nullptr;

	public:
		virtual ~IShape() = default;
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
