#pragma once
#include <float.h>

namespace SV {

	struct Envelope {

		Envelope() {
			MaxX = DBL_MIN;
			MinX = DBL_MAX;
			MaxY = DBL_MIN;
			MinY = DBL_MAX;
		}

		Envelope(double maxX, double minX, double maxY, double minY) {
			MaxX = maxX;
			MinX = minX;
			MaxY = maxY;
			MinY = minY;
		}

		double      MinX;
		double      MaxX;
		double      MinY;
		double      MaxY;
	};
}