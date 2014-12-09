//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    BoundingSphere.h
//	Purpose: Bounding Sphere.
//          Applications should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __BOUNDINGSPHERE_H__
#define __BOUNDINGSPHERE_H__

#include <vector>

/**
 * Sphere
 */
struct BoundingSphere
{
   Point3 m_center;
   float  m_radius;

   /**
    * Default constructor.
    */
   BoundingSphere(void)
   {
      m_center.x = 0.0f;
      m_center.y = 0.0f;
      m_radius   = 1.0f;
   }

   /** 
    * Copy constructor
    * @param   s   Sphere to copy.
    */
   BoundingSphere(const BoundingSphere& s)
   {
      m_center = s.m_center;
      m_radius = s.m_radius;
   }

   /**
    * Constructor given a center point and radius.
    * @param  c  Center point.
    * @param  r  Radius.
    */
   BoundingSphere(const Point3& c, const float r)
   {
      m_center = c;
      m_radius = r;
   }

   /**
    * Construct a sphere given a vertex list. Method by Ritter.
    * @param  vertexList  Vertex list to surround with the sphere.
    */
   BoundingSphere(std::vector<Point3>& vertexList)
   {
      // 605.767
   }

   /**
    * Merge this bounding sphere with another to create the smallest sphere
    * containing the 2.
    * @param  s2  Sphere to merge with this sphere.
    */
   BoundingSphere Merge(BoundingSphere& s2)
   {
      // 605.767
      return BoundingSphere();
   }
};

#endif
