//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Plane.h
//	Purpose: Structures to hold definition of a plane in 3 space.
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//============================================================================

#ifndef __PLANE_H__
#define __PLANE_H__

#include <math.h>

struct Plane
{
   float a, b, c, d;     // Components of the plane equation

   /**
    * Default constructor
    */
   Plane()
   {
      a = 1.0;
      b = 0.0; 
      c = 0.0;
      d = 0.0;
   }

   /**
    * Constructor given a normal vector and a point on the plane.
    * @param  p  Point on the plane
    * @param  n  Normal to the plane
    */
   Plane(const Point3& p, const Vector3& n)
   {
      Set(p, n);
   }  
   
   /**
    * Constructor given 3 points on the plane. Assume the points are
    * in counter-clockwise order to determine the outward direction.
    * @param  p1   1st point on the plane
    * @param  p2   2nd point on the plane
    * @param  p3   3rd point on the plane
    */
   Plane(const Point3& p1, const Point3& p2,const Point3& p3)
   {
      // Construct 2 vectors and find the cross product.
      Vector3 v1(p1, p2);
      Vector3 v2(p1, p3);
      Set(p1, v1.Cross(v2));
   }

   /**
    * Set the plane given a normal vector and a point on the plane
    * @param  p  Point on the plane
    * @param  n  Normal to the plane
    */
   void Set(const Point3& p, const Vector3& n)
   {
      a = n.x;
      b = n.y;
      c = n.z;
      d = (a * p.x + b * p.y + c * p.z);
   }

   /**
    * Normalize the plane equation - divides through by the magnitude of the
    * normal vector to make sure the normal is unit length.
    */
   void Normalize()
   {
      float l = sqrtf(a * a + b * b + c * c);
      float m = (fabs(l) > EPSILON) ? 1.0f / l : 1.0f;
		a *= m;
		b *= m;
		c *= m;
		d *= m;
   }

   /**
    * Solves the plane equation using the supplied point. The sign of the
    * value indicates on which side of the plane the point lies. If the plane
    * is constructed with a unit normal, then the value is the signed distance
    * of the point to the plane.
    * @param  p  Point to solve the plane equation with.
    * @return  Returns the solution of the plane equation given an input point
    */
   float Solve(const Point3& p) const
   {
      return (a * p.x + b * p.y + c * p.z - d);
   }

   /**
    * Returns a normal to the plane
    * @return  Returns a normal to the plane.
    */
   Vector3 GetNormal() const
   {
      return Vector3(a, b, c);
   }
};

#endif
