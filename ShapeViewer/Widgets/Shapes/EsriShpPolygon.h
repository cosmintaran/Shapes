#pragma once

#include "Polygon.h"
#include <ogrsf_frmts.h>

namespace SV::GS {

	class EsriShpPolygon 
		: public Polygon  {

	private:
		OGRPolygon _ogrPolygon;

	public:
		EsriShpPolygon(ILayer* layer, const OGRPolygon* polygon);
		~EsriShpPolygon();

	private:
		void PrepareForDraw();
	};

}
