#pragma once
#include <vector>
#include <string>
#include <glm/vec4.hpp>
#include "Shapes/IShape.h"
#include "Shapes/ShpTypes.h"

namespace SV::GS {

	class DeviceContext;

	struct ShapeSortByMaxX {
		bool operator()(const IShape* p1 , const IShape* p2) {
			return p1->GetBoundingBox().xMax < p2->GetBoundingBox().xMax;
		}
	};

	class Layer {
	private:
		std::string _layerName;
		ShapeType _shapeType;
		glm::vec4 _color;
		glm::vec4 _outlineColor;
		bool _isVisible;
		bool _isDirty;
		std::vector<IShape*> _shapes;
	public:
		Layer(const std::string& name, ShapeType type, glm::vec4 color, glm::vec4 outlineColor = glm::vec4(1.0f));
		virtual ~Layer();
		const glm::vec4& GetColor()const {return _color;}
		const glm::vec4& GetOutlineColor()const { return _outlineColor; }
		uint8_t GetR()const { return static_cast<uint8_t>(_color.r); }
		uint8_t GetG()const { return static_cast<uint8_t>(_color.g); }
		uint8_t GetB()const { return static_cast<uint8_t>(_color.b); }
		uint8_t GetA()const { return static_cast<uint8_t>(_color.a); }
		void AddShape(const std::vector<glm::vec3>&vertices);
		bool IsVisible() const { return _isVisible; }
		bool IsDirty()const { return _isDirty; }
		void SetIsDirty(bool value) { _isDirty = value; }
		void Draw(DeviceContext& context);

	private:
		template< typename T, typename Pred >
		typename std::vector<T>::iterator
			insert_sorted(std::vector<T>& vec, const T& item, Pred pred) {
			return vec.insert
			(
				std::upper_bound(vec.begin(), vec.end(), item, pred), item
			);
		}	 
	};
}