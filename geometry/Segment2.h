//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Segment.h
//	Purpose: 2D Line segment class. Includes clipping code. 
//          Student should  include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __SEGMENT2_H__
#define __SEGMENT2_H__

#include <math.h>
#include <vector>

/**
 * Rectangle
 */
struct CRectangle
{
   float left;
   float right;
   float bottom;
   float top;
};

// Clipping code words for Cohen-Sutherland line segment clipping
union ClipCode
{
   unsigned int code;
   struct {
      unsigned int left : 1;
      unsigned int right : 1;
      unsigned int bottom : 1;
      unsigned int top : 1;
   } m_bits;

   // Constructor. Set code to 0;
   ClipCode(void) : code(0) { }
};

/**
 * Line segment in 2D
 */
struct LineSegment2
{
   Point2 A;
   Point2 B;

   /**
    * Default constructor.
    */
   LineSegment2(void)
   {
      A.Set(0.0f, 0.0f);
      B.Set(0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    First point of the segment.
    * @param   p2    Second point of the segment.
    */
   LineSegment2(const Point2& p1, const Point2& p2)
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
   float Distance(const Point2& P, Point2& closestPt) const
   {
      // Construct vector v (AB) and w (AP)
      Vector2 v(A, B);
      Vector2 w(A, P);

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
         return Vector2(B, P).Norm();
      }

      // Closest point is along the segment. The point is the projection of
      // w onto v.
      closestPt = A + v * (n / d);
      return Vector2(closestPt, P).Norm();
   }

   /**
    * Determines if the current segment intersects the specified segment.
    * If an intersect occurs the intersectPt is determined.  Note: the
    * case where the lines overlap is not considered.
    * @param   segment        Segment to determine intersection with.
    * @param   intersectPt    (OUT) Intersection point.
    * @return   Returns true if an intersection exists, false if not.
    */
   bool Intersect(const LineSegment2& segment, Point2& intersectPt)
   {
      // Construct vectors
      Vector2 b = B - A;
      Vector2 d = segment.B - segment.A;

      // Set 2D perpendicular vector to d
      Vector2 dp(d.y, -d.x);

      // Check if denominator will be 0 (lines are parallel)
      float dtb = dp.Dot(b);
      if (dtb == 0.0f)
         return false;

      // Solve for the parameter t
      Vector2 c = segment.A - A;
      float dtc = dp.Dot(c);
      float t = dtc / dtb;
      if (t < 0.0f || t > 1.0f)
         return false;
      
      // Solve for the parameter u
      Vector2 bp(b.y, -b.x);
      float u = bp.Dot(c) / dtb;
      if (u < 0.0f || u > 1.0f)
         return false;

      // An intersect occurs.  Set the intersect point and return true
      intersectPt = A + b * t;
      return true;
   }

   /**
    * Clips the line segment to a specified convex polygon.
    * @param  poly  A counter-clockwise oriented polygon.
    * @returns   Returns the clipped segment.
    */
   bool ClipToPolygon(std::vector<Point2>& poly, LineSegment2& clipSegment)
   {
      // Initialize the candidate interval
      float tOut = 1.0f;
      float tIn  = 0.0f;

      // Ray for this line segment
      Vector2 c = B - A;
     
      float tHit;
      Vector2 n;
      Vector2 w;
      std::vector<Point2>::iterator pt1 = poly.end() - 1;
      std::vector<Point2>::iterator pt2 = poly.begin();
      for ( ; pt2 != poly.end(); pt1 = pt2, pt2++)
      {
         // Set an outward facing normal (polygon is assumed to be CCW)
         n.Set(pt2->y - pt1->y, pt1->x - pt2->x);   
         
         // Get the dot product of the normal to this polygon edge
         // with the ray
         float nDotc = n.Dot(c);

         // Check for parallel line
         if (fabs(nDotc) < EPSILON)
            continue;

         w = *pt1 - A;
         tHit = n.Dot(w)  / nDotc;
         
         // Ray is exiting P
         if (nDotc > 0)
            tOut = (tOut < tHit) ? tOut : tHit;
         else
            tIn = (tIn > tHit) ? tIn : tHit;

         // Early out
         if (tIn > tOut)
         {
            // Set clip segment to 0s
            clipSegment.A.Set(0.0f, 0.0f);
            clipSegment.B.Set(0.0f, 0.0f);
            return false;
         }
      }

      // If candidate interval is not empty then set the clip segment
      clipSegment.A = A + c * tIn;
      clipSegment.B = A + c * tOut;
      return true;
   }

   /**
    * Clips the line segment to a specified rectangle.
    * @param  poly  A counter-clockwise oriented polygon.
    * @returns   Returns the clipped segment.
    */
   bool ClipToRectangle(const CRectangle& r, LineSegment2& clip)
   {
		ClipCode p1, p2;
		clip.A = A;
		clip.B = B;
		while (true)
      {
         // Form code words for the two points      
         p1.m_bits.left   = (clip.A.x < r.left);
         p1.m_bits.right  = (clip.A.x > r.right);
         p1.m_bits.bottom = (clip.A.y < r.bottom );
         p1.m_bits.top    = (clip.A.y > r.top);
         p2.m_bits.left   = (clip.B.x < r.left);
         p2.m_bits.right  = (clip.B.x > r.right);
         p2.m_bits.bottom = (clip.B.y < r.bottom );
         p2.m_bits.top    = (clip.B.y > r.top);
      
         // Trivial accept - both points are inside, done
         if (p1.code == 0 && p2.code == 0)
            return true;

         // Trivial reject. Both points are outside
         if ((p1.m_bits.left   == 1 && p2.m_bits.left   == 1) ||
             (p1.m_bits.right  == 1 && p2.m_bits.right  == 1) ||
             (p1.m_bits.bottom == 1 && p2.m_bits.bottom == 1) ||
             (p1.m_bits.top    == 1 && p2.m_bits.top    == 1))
         {
            clip.A.Set(0.0f, 0.0f);
            clip.B.Set(0.0f, 0.0f);
            return false;
         }

         if (p1.code != 0)
         {
            if (p1.m_bits.left)
            {
               clip.A.y += (r.left - clip.A.x) * (clip.A.y - clip.B.y) /
                           (clip.A.x - clip.B.x);
               clip.A.x = r.left;
            }
            else if (p1.m_bits.right)
            {   
               clip.A.y += (r.right - clip.A.x) * (clip.A.y - clip.B.y) / 
                           (clip.A.x - clip.B.x);
               clip.A.x = r.right;
            }
            else if (p1.m_bits.bottom)
            {
               clip.A.x += (clip.A.y - r.bottom) * (clip.B.x - clip.A.x) /
                           (clip.A.y - clip.B.y);
               clip.A.y = r.bottom;
            }
            else if (p1.m_bits.top)
            {
               clip.A.x += (clip.A.y - r.top) * (clip.B.x - clip.A.x) /
                           (clip.A.y - clip.B.y);
               clip.A.y = r.top;
            }
        } 
        else
        {
            if (p2.m_bits.left)
            {
               clip.B.y += (r.left - clip.B.x) * (clip.B.y - clip.A.y) / 
                           (clip.B.x - clip.A.x);
               clip.B.x = r.left;
            }
            else if (p2.m_bits.right)       
            {
               clip.B.y += (r.right - clip.B.x) * (clip.B.y - clip.A.y) / 
                           (clip.B.x - clip.A.x);
               clip.B.x = r.right;
            }
            else if (p2.m_bits.bottom)       
            {
               clip.B.x += (clip.B.y - r.bottom) * (clip.A.x - clip.B.x) /
                           (clip.B.y - clip.A.y);
               clip.B.y = r.bottom;
            }
            else if (p2.m_bits.top)   
            {
               clip.B.x += (clip.B.y - r.top) * (clip.A.x - clip.B.x) /
                           (clip.B.y - clip.A.y);
               clip.B.y = r.top;
            }
         }
      }
   }
};

#endif
