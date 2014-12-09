//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Vector2.h
//	Purpose: 2D Vector class. 
//          Student should include "geometry.h" to get all class definitions
//          included in proper order.
//
//============================================================================

#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <stdarg.h>
#include <math.h>

/**
 * 2D vector class
 */
struct Vector2
{
public:
	float x;
   float y;

	/**
    * Default constructor
    */
	Vector2() : x(0.0f), y(0.0f) { }

   /**
    * Constructor given a point.  Essentially a vector from the
    * origin to the point.
    * @param   p  Point.
    */
	Vector2(const Point2& p) : x(p.x), y(p.y) { }

   /**
    * Constructor given 3 components of the vector.
    * @param   ix   x component of the vector.
    * @param   iy   y component of the vector.
    */
  	Vector2(const float ix, const float iy) : x(ix), y(iy) { }

   /**
    * Constructor from one point to another.
    * @param   from  Point at origin of the vector.
    * @param   to    Point at end of vector
    */
	Vector2(const Point2& from, const Point2& to) :
         x(to.x - from.x), y(to.y - from.y) { }

	/**
    * Copy constructor.
    * @param   w  Vector to copy to the new vector.
    */
	Vector2(const Vector2& w) : x(w.x), y(w.y) { }

   /**
    * Assignment operator
    * @param   w  Vector to copy to the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector2& operator = (const Vector2& w)
	{
		x = w.x;
		y = w.y;
		return *this;
	}

	/**
    * Destructor
    */
	~Vector2() { }
	
   /**
    * Set the current vector to the specified components.
    * @param   ix   x component of the vector.
    * @param   iy   y component of the vector.
    */
	void Set(const float ix, const float iy)
	{
		x = ix;
		y = iy; 
	}

   /**
    * Set the vector components to those of a point.  Essentially a 
    * vector from the origin to the point.
    * @param   p  Point.
    */
	void Set(const Point2& p)
	{
		x = p.x;
		y = p.y;
	}

   /**
    * Set the current vector to be from one point to another.
    * @param   from  Point at origin of the vector.
    * @param   to    Point at end of vector
    */
	void Set(const Point2& from, const Point2& to)
	{
      x = to.x - from.x;
      y = to.y - from.y;
	}

	/**
    * Creates a new vector that is the current vector plus the
    * specified vector.
    * @param   w  Vector to add to the current vector.
    * @return   Returns the resulting vector.
    */
	Vector2 operator + (const Vector2& w) const
	{
		return Vector2(x + w.x, y + w.y);
	}

	/**
    * Adds vector w to the current vector.
    * @param   w  Vector to add to the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector2& operator += (const Vector2& w)
	{
	   x += w.x;
		y += w.y;
		return *this;
	}

	/**
    * Creates a new vector that is the current vector minus the
    * specified vector.
    * @param   w  Vector to subtract from the current vector.
    * @return   Returns the resulting vector.
    */
	Vector2 operator - (const Vector2& w) const
	{
      return Vector2(x - w.x, y - w.y);
	}

	/**
    * Subtracts vector w from the current vector.
    * @param   w  Vector to subtract from the current vector.
    * @return  Returns the address of the current vector.
    */
	Vector2& operator -= (const Vector2& w)
	{
      x -= w.x;
		y -= w.y;
		return *this;
	}

	/**
    * Creates a new vector that is the current vector multiplied
    * with the specified scalar.
    * @param   scalar   Scalar to muliply the vector with.
    * @return  Returns the resulting vector
    */
	Vector2 operator * (const float scalar) const
	{
      return Vector2(x * scalar, y * scalar);
	}

	/**
    * Multiplies the current vector by a scalar
    * @param   scalar   Scalar to muliply the vector with.
    * @return  Returns the address of the current vector.
    */
	Vector2& operator *= (const float scalar)
	{
      x *= scalar;
		y *= scalar;
		return *this;
	}

	/**
    * Equality operator.
    * @param   w  Vector to test if equal to the current vector.
    * @return  Returns true if vector w equals the current vector,
    *          false otherwise.
    */
	bool operator == (const Vector2& w) const
	{
      return (x == w.x && y == w.y);
	}

	/**
    * Computes the dot product of the current vector with the 
    * specified vector.
    * @param   w  Vector
    * @return  Returns the dot product (a scalar).
    */
	float Dot(const Vector2& w) const
	{
       return (x * w.x + y * w.y);
	}

	/**
    * Computes the 2D cross product of current vector with w0.
    * @param   w  Vector to take the cross product with (current X w)
    * @return  Returns the magnitude of the resulting vector (which is
    *          along the z axis)
    */
	float Cross(const Vector2& w) const
	{
       return (x * w.y - y * w.x);
	}

   /**
    * Get a perprendicular vector to this vector.
    * @param  clockwise  If true get the clockwise oriented perpendicular.
    *                    If false (default) get the counter-clockwise oriented perpendicular.
    */
   Vector2 GetPerpendicular(const bool clockwise = false) const
   {
      return (clockwise) ? Vector2(y, -x) : Vector2(-y, x);
   }

	/**
    * Computes the norm (length) of the current vector.
    * @return  Returns the length of the vector.
    */
	float Norm() const
	{
      return sqrtf(Dot(*this));
	}

	/**
    * Computes the squared norm of a vector
    * (Useful when absolute distance is not required)
    * @return  Returns the length squared of the vector.
    */
	float NormSquared(void) const
	{
      return (Dot(*this));
	}

	/**
    * Normalizes the vector.
    * @return  Returns the address of the current vector.
    */
	Vector2& Normalize()
	{
      // Normalize the vector if the norm is not 0 or 1
		float n = Norm();
		if (n > EPSILON && n != 1.0f)
		{
			x /= n;
			y /= n;
		}
		return *this;
	}

	/**
    * Calculates the component of the current vector along the 
    * specified vector
    * @param   w  Vector to determine component along.
    * @return  Returns the component of the current vector along w.
    */
	float Component(const Vector2& w) const
	{
      float n = w.Dot(w);
		return (n != 0.0f) ? (Dot(w) / n) : 0.0f;
	}

	/**
    * Creates a new vector that is the projection of the current 
    * vector along the specified vector.
    * @param   w  Vector to determine projection along.
    * @return  Returns the new vector.
    */
	Vector2 Projection(const Vector2& w) const
	{
      return w * Component(w);
	}

	/**
    * Calculates the angle (radians) between the current vector and 
    * the specified vector.
    * @param   w  Vector to determine angle from current vector.
    * @return  Returns the angle in radians between the two vectors.
    */
	float AngleBetween(const Vector2& w) const
	{
      return acosf(Dot(w) / (Norm() * w.Norm()));
	}

   /**
    * Reflects the current vector given a normal to the reflecting surface.
    * Assumes the normal is unit length.  Note that if done properly the
    * magnitude of the reflected vector will equal the magnitude of the
    * incoming vector.
    * @param   normal   unit length normal to the vector where reflection occurs
    * @return  Returns the reflected vector
    */
   Vector2 Reflect(const Vector2& normal) const
   {
      Vector2 d = *this;
      return (d - (normal * (2.0f * (d.Dot(normal)))));
   }
};

#endif
