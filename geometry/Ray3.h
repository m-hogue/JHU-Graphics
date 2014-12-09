//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Ray.h
//	Purpose: Ray class. Includes intersection methods with other objects.
//          Student should include "geometry.h" to get all class definitions
//          included in proper order.
//============================================================================

#ifndef __RAY_H__
#define __RAY_H__

#include <math.h>
#include <vector>

/**
 * 3D ray
 */
struct Ray3
{
   Point3  o;     // Origin of the ray
   Vector3 d;     // Ray direction

   /**
    * Default constructor.
    */
   Ray3()
   {
      o.Set(0.0f, 0.0f, 0.0f);
      d.Set(1.0f, 0.0f, 0.0f);
   }

   /**
    * Constructor given 2 points.
    * @param   p1    Start point of the ray.
    * @param   p2    End point of the ray (at t = 1).
    * @param   norm  If true normalize the direction vector
    */
   Ray3(const Point3& p1, const Point3& p2, bool normalize)
   {
      o = p1;
      d = p2 - p1;
      if (normalize && d.NormSquared() != 1.0f)
         d.Normalize();
   }
 
   /**
    * Constructor given an origin and a direction. 
    * @param   origin   Origin point of the ray.
    * @param   dir      Ray direction.
    */
   Ray3(const Point3& origin, const Vector3& dir)
   {
      o = origin;
      d = dir;
   }

   /**
    * Constructor given an origin and a direction. Normalizes the 
    * ray direction.
    *
    * @param   origin   Origin point of the ray.
    * @param   dir      Ray direction.
    */
   Ray3(const Point3& origin, const Vector3& dir, bool normalize)
   {
      o = origin;
      d = dir;
      if (normalize)
         d.Normalize();
   }

   /** 
    * Calculates the reflected ray given the intersect point and
    * the normal to the surface. 
    * @return  Returns the reflected ray.
    */
   Ray3 Reflect(const Point3& intPt, const Vector3& normal) const
   {
      // Required in 605.767
      return Ray3();
   }

   /**
    * Finds the refracted ray.
    * @param   intPt   Intersection point (origin of the refracted ray)
    * @param   n       Normal at the intersection point
    * @param   u1      Index of refraction (leaving)
    * @param   u2      Index of refraction (entering)
    * @param   tir     (OUT) Total internal reflection
    * @return  Returns the refracted ray.
    */
   Ray3 Refract(const Point3& intPt, Vector3& n, const float u1, float u2, 
                bool& tir)
   {
      // Required in 605.767
      return Ray3();
   }

   /**
    * Returns the intersect point given the parameter t
    * @param   t    Parameter along the ray
    * @return  Returns the point along the direction d from the origin a.
    */
   Point3 Intersect(const float t) const
   {
      return o + d * t;
   }

   //  ---------------- Intersection methods --------------------//

   /**
    * Intersection of the ray with a plane.
    * @param   plane  Plane to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const Plane& p) const
   {
     // Required in 605.767
      return 0.0f; 
   }
      
   /**
    * Intersection of a ray and a sphere using vector method.
    * @param   sphere  Sphere to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const BoundingSphere& sphere) const
   {
      // Construct vector from ray origin to sphere center. Get squared length
      Vector3 l = sphere.m_center - o;
      float l2  = l.NormSquared();

      // Component of l onto ray. Since the ray direction is unit length, the component
      // is the distance along the ray to the closest point to the sphere (perp.)
      float s  = l.Dot(d);
      float r2 = SQR(sphere.m_radius);
      if (s < 0.0f && l2 > r2)
      {
         // Sphere center is behind ray origin AND ray origin
         // is outside sphere
         return 0.0f;
      }
   
      // Distance (squared) of closest point along the ray (perpendicular)
      float m2 = l2 - (s*s);                           
      if (m2 > r2)                  // Ray passes outside the sphere
         return 0.0f;
   
      float q = sqrtf(r2-m2);
      if (l2 - r2 > EPSILON)
      {
         // Ray origin is outside sphere: nearest intersection is at 
         // value t=s-q
         return s - q;
      }
      else                          // Ray origin is inside sphere
         return s + q;
   }

   /**
    * Intersection of a ray and an axis aligned bounding box.
    * @param   box  AABB to test intersection with
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const AABB& box) const
   {
      // Required in 605.767
      return 0.0f; 
   }

   /**
    * Intersection of a ray and a polygon
    * @param   polygon  Polygon to test intersection with
    * @param   normal   Normal to the polygon
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(std::vector<Point3>& polygon, Vector3& normal) const
   {
      // Required in 605.767
      return 0.0f;
   }

   /**
    * Intersection of a ray with a triangle.
    * @param   v0  Vertex of the triangle
    * @param   v1  Vertex of the triangle
    * @param   v2  Vertex of the triangle
    * @param   u   (OUT) barycentric coordinate of intersection
    * @param   v   (OUT) barycentric coordinate of intersection
    * @return  Returns the parameter t along the ray where intersection occurs. Return
    *          0.0f if no intersection occurs.
    */
   float Intersect(const Point3& v0, const Point3& v1, const Point3& v2,
                   float& u, float& v) const
   {
      // Required in 605.767
      return 0.0f;
   }
};

#endif
