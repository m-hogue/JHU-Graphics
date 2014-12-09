#ifndef FITTING_H_
#define FITTING_H_

#include "Scene/Scene.h"

class Fitting : public TriSurface {
public:
	/**
	 * Construct a generic fitting object by means of a surface of revoltion
	 */
	Fitting(const int positionLoc, const int normalLoc, const int textureLoc){
		int ang, i;
		int delang = 10;
		float r[12] = { 0.0, 0.3, 0.3, 0.25, 0.25, 0.35, 0.35, 0.3, 0.15, 0.15, 0.05, 0.0 };
		float y[12] = { 0.0, 0.0, 0.05, 0.1, 0.3, 0.35, 0.45, 0.5, 0.5, 0.4, 0.3, 0.3 };
		float x1, x2, z1, z2, x3, z3, x4, z4;
		for (i = 0; i<11; i++)
		{
			for (ang = 0; ang <= 360; ang += delang)
			{
				x1 = r[i] * cos((double)ang*2.0*M_PI / 360.0);
				z1 = r[i] * sin((double)ang*2.0*M_PI / 360.0);
				x2 = r[i + 1] * cos((double)ang*2.0*M_PI / 360.0);
				z2 = r[i + 1] * sin((double)ang*2.0*M_PI / 360.0);
				x3 = r[i] * cos((double)(ang + delang)*2.0*M_PI / 360.0);
				z3 = r[i] * sin((double)(ang + delang)*2.0*M_PI / 360.0);
				x4 = r[i + 1] * cos((double)(ang + delang)*2.0*M_PI / 360.0);
				z4 = r[i + 1] * sin((double)(ang + delang)*2.0*M_PI / 360.0);
				Point3 p1 = Point3(x1, y[i], z1);
				Point3 p2 = Point3(x2, y[i + 1], z2);
				Point3 p3 = Point3(x3, y[i], z3);
				Point3 p4 = Point3(x4, y[i + 1], z4);
				
				Add(p2, p3, p1);
				Add(p2, p4, p3);
			};
		}
		End(positionLoc, normalLoc, textureLoc);
	}

	~Fitting(){ }

protected:

};


#endif