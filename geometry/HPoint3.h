//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Point.h
//	Purpose: Structures to hold 3D homogenous coordinates
//
//============================================================================

#ifndef __HPOINT3_H__
#define __HPOINT3_H__

#include <math.h>

// Forward reference
struct Point3;

/**
 * Homogeneous coordinate
 */
struct HPoint3
{
	float x;
   float y; 
   float z;
   float w;

	/**
    * Default constructor
    */
	HPoint3() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }

   /**
    * Constructor with initial values for x,y,z,w.
    *
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    * @param   iz   z coordinate position.
    * @param   iw   Homogeneous factor
    */
   HPoint3(const float ix, const float iy, const float iz, const float iw)
               : x(ix), y(iy), z(iz), w(iw)  { }

   /**
    * Convert to a cartesian representation
    * @return  Returns the cartesian representation of this point.
    */
   inline Point3 ToCartesian() const;
};

#endif
