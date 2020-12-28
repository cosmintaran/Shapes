#pragma once
#include "IShape.h"
#include <vector>

namespace SV::GS {

	class Polygon : public IShape {

	private:
		Polygon()=delete;

	protected:
		Polygon(const ILayer* layer);
		void Triangulate();
	};

}