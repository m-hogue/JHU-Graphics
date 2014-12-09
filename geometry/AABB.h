//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    AABB.h
//	Purpose: Axis Aligned Bounding Box (3D).
//          Applications should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __AABB_H__
#define __AABB_H__

#include <vector>

// NOTE - this is not required until 605.767!

/**
 * Axis Aligned Bounding Box.
 */
struct AABB
{
   /**
    * Default constructor.
    */
   AABB()
   {
   }

   /**
    * Construct an AABB given a minimum and maximum point.
    * @param  minPt  Minimum point (x,y,z)
    * @param  maxPt  Maximum point (x,y,z)
    */
   AABB(const Point3& minPt, const Point3& maxPt)
   {
   }

   /**
    * Construct an AABB given a vertex list.
    * @param  vertexList  Vertex list.
    */
   AABB(std::vector<Point3>& vertexList)
   {
   }

   /**
    * Creates an AABB given a vertex list.
    * @param  vertexList  Vertex list.
    */
   void Create(std::vector<Point3>& vertexList)
   {
   }

   /**
    * Get the point at the minimum x,y,z.
    * @return  Returns the min. point.
    */
   Point3 GetMinPt() const
   {
      return Point3();
   }

   /**
    * Get the point at the maximum x,y,z.
    * @return  Returns the max. point.
    */
   Point3 GetMaxPt() const
   {
      return Point3();
   }
};

#endif
