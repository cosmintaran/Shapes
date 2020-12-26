#include "stdafx.h"
#include "Triangulator.h"

extern "C"
{
#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

#include "triangle.h"
}

namespace SV::GS {

	void Triangulator::TriangulatePolygon(const std::vector<glm::vec3>& polygon)
	{
		if (polygon.size() <= 3) return;
		triangulateio in, mid, vorout;

		const int noOfPoints = (int)polygon.size();
		const int noOfAttributes = 1;
		const int pointListSize = noOfPoints * 2;
		const int pointAttribListSize = noOfPoints * noOfAttributes;

		in.numberofpoints = noOfPoints;
		in.numberofpointattributes = noOfAttributes;
		in.pointlist = (REAL*)malloc(noOfPoints * 2 * sizeof(REAL));
		in.pointattributelist = (REAL*)malloc(in.numberofpoints *
			in.numberofpointattributes *
			sizeof(REAL));

		int j = 0;
		for (int i = 0; i < noOfPoints; ++i) {

			in.pointlist[j] = polygon[i].x;
			in.pointlist[++j] = polygon[i].y;
			++j;
		}

		for (int i = 0; i < pointAttribListSize; ++i) {
			in.pointattributelist[i] = i;
		}

		mid.pointlist = (REAL*)NULL;            /* Not needed if -N switch used. */
		/* Not needed if -N switch used or number of point attributes is zero: */
		mid.pointattributelist = (REAL*)NULL;
		mid.pointmarkerlist = (int*)NULL; /* Not needed if -N or -B switch used. */
		mid.trianglelist = (int*)NULL;          /* Not needed if -E switch used. */
		/* Not needed if -E switch used or number of triangle attributes is zero: */
		mid.triangleattributelist = (REAL*)NULL;
		mid.neighborlist = (int*)NULL;         /* Needed only if -n switch used. */
		/* Needed only if segments are output (-p or -c) and -P not used: */
		mid.segmentlist = (int*)NULL;
		/* Needed only if segments are output (-p or -c) and -P and -B not used: */
		mid.segmentmarkerlist = (int*)NULL;
		mid.edgelist = (int*)NULL;             /* Needed only if -e switch used. */
		mid.edgemarkerlist = (int*)NULL;   /* Needed if -e used and -B not used. */

		vorout.pointlist = (REAL*)NULL;        /* Needed only if -v switch used. */
		/* Needed only if -v switch used and number of attributes is not zero: */
		vorout.pointattributelist = (REAL*)NULL;
		vorout.edgelist = (int*)NULL;          /* Needed only if -v switch used. */
		vorout.normlist = (REAL*)NULL;         /* Needed only if -v switch used. */

		/* Triangulate the points.  Switches are chosen to read and write a  */
		/*   PSLG (p), preserve the convex hull (c), number everything from  */
		/*   zero (z), assign a regional attribute to each element (A), and  */
		/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
		/*   neighbor list (n).                                              */

		char cmd[] = "pczAevn";
		triangulate(cmd, &in, &mid, NULL);
		//triangulate();
		//trifree();
		delete[]  in.pointlist;

	}

}