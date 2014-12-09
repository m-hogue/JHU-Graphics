//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Point2.h
//	Purpose: Structure to hold 2D coordinates
//
//============================================================================

#ifndef __POINT2_H__
#define __POINT2_H__

#include <math.h>

// Forward references
struct Vector2;

/**
 * 2D Point (cartesian)
 */
struct Point2
{
	float x;
   float y; 

	/**
    * Default constructor
    */
	Point2() : x(0.0f), y(0.0f) { }

   /**
    * Constructor with initial values for x,y.
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    */
   Point2(const float ix, const float iy) : x(ix), y(iy) { }

	/**
    * Copy constructor.
    * @param   p   Point to copy to the new point.
    */
	Point2(const Point2& p) : x(p.x), y(p.y) { }

   /**
    * Convert a homogeneous coordinate into a cartesian coordinate.
    * @param   p   Homogenous point
    */
	Point2(const HPoint2& p) 
   { 
      *this = p.ToCartesian(); 
   }

   /**
    * Assignment operator
    * @param   p   Point to assign to this point.
    * @return  Returns the address of this point.
    */
	Point2& operator = (const Point2& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	/**
    * Destructor
    */
	~Point2() { }

   /**
    * Set the coordinate components to the specified values.
    * @param   ix   x coordinate position.
    * @param   iy   y coordinate position.
    */
	void Set(const float ix, const float iy)
	{
		x = ix;
		y = iy;
	}

   /**
    * Equality operator.
    * @param   p  Point to compare to the current point.
    * @return  Returns true if two points are equal, false otherwise.
    */
	bool operator == (const Point2& p) const
	{
		return (x == p.x && y == p.y);
   }

   /**
    * Affine combination of this point with another point. 2 scalars are provided
    * (a0 and a1) and the must add to 1.
    * @param  a0  Scalar for this point
    * @param  a1  Scalar for p1
    * @param  p1  Point 1
    */
   Point2 affineCombination(const float a0, const float a1, const Point2& p1) const
   {
      return Point2(a0*x + a1*p1.x, a0*y + a1*p1.y);
   }

   /**
    * Gets the midpoint on a line segment between this point and point p1.
    * @param  p1  Point
    * @return  Returns the midpoint between this point and p1.
    */
   Point2 midPoint(const Point2& p1) 
   {
      return Point2(0.5f*x + 0.5f*p1.x, 0.5f*y + 0.5f*p1.y);
   }

   // The following methods are defined in geometry.h

   /**
    * Add a vector to the current point.
    * @param   v  Vector to add to the current point.
    * @return  Returns a new point: the result of the current point
    *          plus the specified vector.
    */
	Point2 operator + (const Vector2& v) const;

   /**
    * Subtract a vector from the current point.
    * @param   v  Vector to subtract from the current point.
    * @return  Returns a new point: the result of the current point
    *          minus the specified vector.
    */
	Point2 operator - (const Vector2& v) const;

   /**
    * Subtraction of a point from the current point.
    * @param   Point to subtract from the current point.
    * @return  Returns a vector.
    */
	Vector2 operator - (const Point2& p) const;
};

#endif
