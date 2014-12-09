//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Point.h
//	Purpose: Structure to hold 2D homogenous coordinates
//
//============================================================================

#ifndef __HPOINT2_H__
#define __HPOINT2_H__

#include <math.h>

// Forward reference
struct Point2;

/**
 * 2D Homogeneous coordinate
 */
struct HPoint2
{
	float x;
   float y; 
   float w;

	/**
    * Default constructor
    */
	HPoint2() : x(0.0f), y(0.0f), w(1.0f) { }

   /**
    * Constructor with initial values for x,y,w.
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    * @param   iw   Homogeneous factor
    */
   HPoint2(const float ix, const float iy, const float iw)
               : x(ix), y(iy), w(iw)  { }

   /**
    * Convert to a cartesian representation
    * @return  Returns the cartesian representation of this point.
    */
   inline Point2 ToCartesian() const;
};

#endif
