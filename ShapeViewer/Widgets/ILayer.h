#pragma once

#include <glm/vec4.hpp>
#include "Shapes/IShape.h"
#include "Shapes/ShpTypes.h"
#include "Envelope.h"

namespace SV::GS {

	class ILayer {

	protected:

		std::string _name;
		glm::vec4 _color;
		glm::vec4 _outlineColor;
		bool _isVisible = true;
		bool _isDirty = true;
		std::vector<IShape*> _shapes;
		Envelope _envelope;

	public:
		const glm::vec4& GetColor()const { return _color; }
		const glm::vec4& GetOutlineColor()const { return _outlineColor; }
		uint8_t GetR()const { return static_cast<uint8_t>(_color.r); }
		uint8_t GetG()const { return static_cast<uint8_t>(_color.g); }
		uint8_t GetB()const { return static_cast<uint8_t>(_color.b); }
		uint8_t GetA()const { return static_cast<uint8_t>(_color.a); }
		
		bool IsVisible() const { return _isVisible; }
		bool IsDirty()const { return _isDirty; }
		void SetIsDirty(bool value) { _isDirty = value; }
		[[nodiscard]] virtual const Envelope& GetEnvelope()const { return _envelope; }
		virtual void Draw(DeviceContext& context);
		virtual ~ILayer();


	protected:
		ILayer(const char* name, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
	};

}
