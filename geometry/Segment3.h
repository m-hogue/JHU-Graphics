//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Segment.h
//	Purpose: 3D Line segment class.
//          Student should  include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __SEGMENT3_H__
#define __SEGMENT3_H__

#include <math.h>
#include <vector>

/**
 * Line segment in 3D
 */
struct LineSegment3
{
   Point3 A;
   Point3 B;

   /**
    * Default constructor.
    */
   LineSegment3(void)
   {
      A.Set(0.0f, 0.0f, 0.0f);
      B.Set(0.0f, 0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    First point of the segment.
    * @param   p2    Second point of the segment.
    */
   LineSegment3(const Point3& p1, const Point3& p2)
   {
      A = p1;
      B = p2;
   }
      
   /**
    * Finds the distance of a specified point from the line segment 
    * and the closest point on the segement to the specified point.
    * @param   P           Test point.
    * @param   closestPt   (Return) Closest point on the segment to c.
    * @return  Returns the distance from P to the closest point on
    *          the segment.
    */
   float Distance(const Point3& P, Point3& closestPt) const
   {
      // Construct vector v (AB) and w (AP)
      Vector3 v(A, B);
      Vector3 w(A, P);

      // Numerator of the component of w onto v. If <= 0 then A 
      // is the closest point. By separating into the numerator
      // and denominator of the component we avoid a division unless
      // it is necessary.
      float n = w.Dot(v);
      if (n <= 0.0f)
      {
         closestPt = A;
         return w.Norm();
      }

      // Get the denominator of the component.  If the component >= 1
      // (d <= n) then point B is the closest point
      float d = v.Dot(v);
      if (d <= n)
      {
         closestPt = B;
         return Vector3(B, P).Norm();
      }

      // Closest point is along the segment. The point is the projection of
      // w onto v.
      closestPt = A + v * (n / d);
      return Vector3(closestPt, P).Norm();
   }
};

#endif
