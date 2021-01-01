#pragma once

#include "Polygon.h"
#include <ogrsf_frmts.h>

namespace SV::GS {

	class EsriShpPolygon 
		: public Polygon  {

	private:
		OGRPolygon _ogrPolygon;

	public:
		EsriShpPolygon(const ILayer* layer, OGRPolygon polygon);
		~EsriShpPolygon();
		void Read()override;
		void vDraw(DeviceContext& deviceContext)const override ; 
	private:
		
	};

}
