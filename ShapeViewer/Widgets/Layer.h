#pragma once
#include "Shapes/ShpTypes.h"
#include "Envelope.h"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace SV{
	namespace Shapes { class Drawable; }
	namespace GS { class DeviceContext; }

	class Layer {

	protected:
		std::string _name;
		glm::vec4 _color;
		glm::vec4 _outlineColor;
		bool _isVisible = true;
		bool _isDirty = true;
		std::vector<Shapes::Drawable*> _shapes;
		Envelope _envelope;

	public:
		const glm::vec4& GetColor()const { return _color; }
		const glm::vec4& GetOutlineColor()const { return _outlineColor; }
		uint8_t GetR()const { return static_cast<uint8_t>(_color.r); }
		uint8_t GetG()const { return static_cast<uint8_t>(_color.g); }
		uint8_t GetB()const { return static_cast<uint8_t>(_color.b); }
		uint8_t GetA()const { return static_cast<uint8_t>(_color.a); }

		//std::vector<Shapes::Drawable*>::iterator* begin() { return &_shapes.begin(); }
		//std::vector<Shapes::Drawable*>::const_iterator* cbegin()const { return &_shapes.cbegin(); }
		//std::vector<Shapes::Drawable*>::iterator* end() { return &_shapes.end(); }
		//std::vector<Shapes::Drawable*>::const_iterator* cend() { return &_shapes.cend(); }
		Shapes::Drawable* operator[](size_t i){ return _shapes[i];}
		[[nodiscard]] size_t Count()const { return _shapes.size(); }

		bool IsVisible() const { return _isVisible; }
		bool IsDirty()const { return _isDirty; }
		void SetIsDirty(bool value) { _isDirty = value; }
		[[nodiscard]] virtual const Envelope& GetEnvelope()const { return _envelope; }
		virtual void Draw(GS::DeviceContext& deviceContext);
		bool IsPointInLayer(const glm::vec3& point)const;
		virtual ~Layer();


	public:
		Layer(const char* name, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
	};

}
