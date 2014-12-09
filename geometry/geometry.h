//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    geometry.h
//	Purpose: Geometric types used in the lab.  Includes
//			   coordinate and vector structures.
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdarg.h>
#include <math.h>
#include <vector>
#include <stdlib.h> 

// M_PI may not be defined
#ifndef M_PI
#define M_PI 3.14159265359
#endif

const float EPSILON = 0.000001f;

#define MAXV(a,b) (((a) > (b)) ? (a) : (b))
#define MINV(a,b) (((a) < (b)) ? (a) : (b))
#define SQR(x)    ((x) * (x))

/**
 * Degrees to radians conversion
 * @param   d   Angle in degrees.
 * @return   Returns the angle in radians.
 */
inline float degreesToRadians(const float d)
{
	return d * ((float)M_PI / 180.0f);
}

/**
 * Radians to degrees conversion
 * @param   r   Angle in radians.
 * @return   Returns the angle in degrees.
 */
inline float radiansToDegrees(const float r)
{
	return r * (180.0f / (float)M_PI);
}

/**
 * Get a random number between 0 and 1
 */
inline float rand01()
{
   return (float)rand() / (float)RAND_MAX;
}

/** 
 * Fast inverse sqrt method. Originally used in Quake III 
 * @param  x  Value to find inverse sqrt for
 * @return  Returns 1/sqrtf(x)
 */
inline float FastInvSqrt(float x)
{
   float xhalf = 0.5f * x;
   int i = *(int*)&x;            // get bits for floating value
   i = 0x5f3759df - (i>>1);      // give initial guess y0
   x = *(float*)&i;              // convert bits back to float
   return x*(1.5f - xhalf*x*x);  // newton step
   // x *= 1.5f - xhalf*x*x;     // repeating step increases accuracy
}

// Include individual geometry files
#include "geometry/HPoint2.h"
#include "geometry/Point2.h"
#include "geometry/HPoint3.h"
#include "geometry/Point3.h"
#include "geometry/Vector2.h"
#include "geometry/Vector3.h"
#include "geometry/Segment2.h"
#include "geometry/Segment3.h"
#include "geometry/Plane.h"
#include "geometry/AABB.h"
#include "geometry/BoundingSphere.h"
#include "geometry/Ray3.h"
#include "geometry/Noise.h"
#include "geometry/Matrix.h"

/**
 * Structure to hold a vertex position and normal
 */
struct VertexAndNormal
{
	Point3  m_vertex;
   Vector3 m_normal;
	
	VertexAndNormal()
	{
		m_vertex.Set(0.0f, 0.0f, 0.0f);
      m_normal.Set(0.0f, 0.0f, 0.0f);
	}
	VertexAndNormal(const Point3& vIn)
	{
      m_vertex = vIn;
		m_normal.Set(0.0f, 0.0f, 0.0f);
	}
};

// Define the operators in Point2 that allow a vector to be added
// to and subtracted from a point. Descriptions are in the Point2
// structure.
inline Point2 Point2::operator + (const Vector2& w) const
{
	return Point2(x + w.x, y + w.y);
}
inline Point2 Point2::operator - (const Vector2 &w) const
{
	return Point2(x - w.x, y - w.y);
}
inline Vector2 Point2::operator - (const Point2& p) const
{
	return Vector2(x - p.x, y - p.y);
}

/**
 * Overloading: allows float * Vector2
 */
inline Vector2 operator *(float s, const Vector2 &v)
{
   return Vector2(v.x * s, v.y * s);
}

// Define the operators in Point3 that allow a vector to be added
// to and subtracted from a point. Descriptions are in the Point3
// structure.
inline Point3 Point3::operator + (const Vector3& w) const
{
	return Point3(x + w.x, y + w.y, z + w.z);
}
inline Point3 Point3::operator - (const Vector3 &w) const
{
	return Point3(x - w.x, y - w.y, z - w.z);
}
inline Vector3 Point3::operator - (const Point3& p) const
{
	return Vector3(x - p.x, y - p.y, z - p.z);
}

/**
 * Overloading: allows float * Vector3
 */
inline Vector3 operator *(float s, const Vector3 &v)
{
   return Vector3(v.x * s, v.y * s, v.z * s);
}

// Convert homogeneous point to a cartesian representation
inline Point3 HPoint3::ToCartesian() const
{
   if (w == 1.0f)
      return Point3(x, y, z);
   else
   {
      // Perform division through by w
      float d = (fabs(w) > EPSILON) ? (1.0f / w) : 1.0f;
      return Point3(x * d, y * d, z * d);
   }
}

// Convert homogeneous point to a cartesian representation
inline Point2 HPoint2::ToCartesian() const
{
   if (w == 1.0f)
      return Point2(x, y);
   else
   {
      // Perform division through by w
      float d = (fabs(w) > EPSILON) ? (1.0f / w) : 1.0f;
      return Point2(x * d, y * d);
   }
}

#endif
