#pragma once
#include "IShape.h"

namespace SV::GS {

	class Polygon : public IShape {
	private:
		Polygon() = delete;

	protected:
		Polygon(const ILayer* layer);
		void Triangulate();
		void PolyPart_Triangulate();
	};

} // namespace SV::GS
