#ifndef __TEST_H
#define __TEST_H

#include "geometry/geometry.h"
#include "Scene/Scene.h"


class test {
public:
	test() {
		dostuff();
	}

protected:
	void dostuff(){
		Point3 v0(5, -3, 5);
		Point3 v1(5, -5, -1);
		Point3 v2(7, -4, -3);
		Plane p(v0, v1, v2);
		
		Point3 E(7, -3, 2);

		float t = p.Solve(E);
	}


};


#endif