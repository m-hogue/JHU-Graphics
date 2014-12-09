//============================================================================
//	Johns Hopkins University Engineering for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt

//	Author:  David W. Nesbitt
//	File:    Matrix.h
//	Purpose: Matrix class
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __MATRIX4x4_H__
#define __MATRIX4x4_H__

#include <math.h>

/**
 * 4x4 matrix. All matrix elements (row, col) are indexed base 0.
 */
class Matrix4x4
{
public:
	/**
    * Constructor.  Sets the matrix to the identity matrix
    */
	Matrix4x4()
	{
      SetIdentity();
	}

   /**
    * Sets the matrix to the identity matrix.
    */
	void SetIdentity(void)
	{
      a[0] = 1.0f; a[4] = 0.0f; a[8]  = 0.0f; a[12] = 0.0f;
      a[1] = 0.0f; a[5] = 1.0f; a[9]  = 0.0f; a[13] = 0.0f;
      a[2] = 0.0f; a[6] = 0.0f; a[10] = 1.0f; a[14] = 0.0f;
      a[3] = 0.0f; a[7] = 0.0f; a[11] = 0.0f; a[15] = 1.0f;
	}

	/**
    * Copy constructor
    * @param  n  Matrix to copy
    */
	Matrix4x4(const Matrix4x4& n)
	{
		m00() = n.m00(); m01() = n.m01(); m02() = n.m02(); m03() = n.m03();
      m10() = n.m10(); m11() = n.m11(); m12() = n.m12(); m13() = n.m13();
      m20() = n.m20(); m21() = n.m21(); m22() = n.m22(); m23() = n.m23();
      m30() = n.m30(); m31() = n.m31(); m32() = n.m32(); m33() = n.m33();
	}

   /**
    * Assignment operator
    * @param   n  Matrix to assign to this matrix
    * @return  Returns the address of this matrix.
    */
	Matrix4x4& operator = (const Matrix4x4& n)
	{
      m00() = n.m00(); m01() = n.m01(); m02() = n.m02(); m03() = n.m03();
      m10() = n.m10(); m11() = n.m11(); m12() = n.m12(); m13() = n.m13();
      m20() = n.m20(); m21() = n.m21(); m22() = n.m22(); m23() = n.m23();
      m30() = n.m30(); m31() = n.m31(); m32() = n.m32(); m33() = n.m33();
		return *this;
	}

   /**
    * Equality operator
    * @param   n  Matrix to test for equality with this matrix.
    * @return  Returns true if hte matrices are equal, false otherwise..
    */
	bool operator == (const Matrix4x4& n) const
	{
      return (m00() == n.m00() && m01() == n.m01() && m02() == n.m02() && m03() == n.m03() &&
              m10() == n.m10() && m11() == n.m11() && m12() == n.m12() && m13() == n.m13() &&
              m20() == n.m20() && m21() == n.m21() && m22() == n.m22() && m23() == n.m23() &&
              m30() == n.m30() && m31() == n.m31() && m32() == n.m32() && m33() == n.m33());
	}

	/**
    * Destructor
    */
	~Matrix4x4(void) { }

   /**
    * Set the matrix to the values specified in the array.
    * @param  m  Array of float values to fill in this matrix. The
    *            elements are arranged in column order.
    */
   void Set(const float* m)
   {
      for (int i = 0; i < 16; i++)
         a[i] = m[i];
   }

   /**
    * Gets the matrix (can be passed to OpenGL)
    * @return   Returns the elements of this matrix in column order.
    */
   const float* Get(void) const
   {
      return a;
   }

   // Read-only access functions
   float m00() const { return a[0];  }
	float m01() const { return a[4];  }
	float m02() const { return a[8];  }
	float m03() const { return a[12]; }
	float m10() const { return a[1];  }
	float m11() const { return a[5];  }
	float m12() const { return a[9];  }
	float m13() const { return a[13]; }
	float m20() const { return a[2];  }
	float m21() const { return a[6];  }
	float m22() const { return a[10]; }
	float m23() const { return a[14]; }
	float m30() const { return a[3];  }
	float m31() const { return a[7];  }
	float m32() const { return a[11]; }
	float m33() const { return a[15]; }

   // Read-write access functions
   float& m00() { return a[0];  }
   float& m01() { return a[4];  }
   float& m02() { return a[8];  }
   float& m03() { return a[12]; }
   float& m10() { return a[1];  }
   float& m11() { return a[5];  }
   float& m12() { return a[9];  }
	float& m13() { return a[13]; }
	float& m20() { return a[2];  }
	float& m21() { return a[6];  }
	float& m22() { return a[10]; }
	float& m23() { return a[14]; }
	float& m30() { return a[3];  }
	float& m31() { return a[7];  }
	float& m32() { return a[11]; }
	float& m33() { return a[15]; } 

   /**
    * Gets a matrix element given by row,column. NOTE: this method takes 
    * row, col as 0 based elements (so it works with the Inverse method)
    * while the individual matrix access methods use a 1-based indexing.
    * @param  row   Matrix row (0-based)
    * @param  col   Matrix col (0-based)
    * @return Returns the element at the specified row,col.
    */
   float m(const unsigned int row, const unsigned int col) const
   {
      return (row < 4 && col < 4) ? a[col * 4 + row] : 0.0f;
   }

   /**
    * Gets a matrix element given by row,column.
    * @param  row   Matrix row (0-based)
    * @param  col   Matrix col (0-based)
    * @return Returns the address of the element at the specified row,col.
    */
   float& m(const unsigned int row, const unsigned int col)
   {
      return (row < 4 && col < 4) ? a[col * 4 + row] : a[0];
   }

   /**
    * Matrix multiplication.  Multiplies the current matrix by the matrix n
    * ( m' = m n ) and returns the resulting 4x4 matrix.
    * @param   n   Matrix to multiply the current matrix by
    * @return  Returns the product of the current matrix and the supplied matrix.
    */
   Matrix4x4 operator * (const Matrix4x4& n) const
   {
      // Unroll the loop, do 1 row at a time.
      Matrix4x4 t;
      float a0 = m00();
      float a1 = m01();
      float a2 = m02();
      float a3 = m03();
      t.m00() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
      t.m01() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
      t.m02() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
      t.m03() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
      a0 = m10();
      a1 = m11();
      a2 = m12();
      a3 = m13();
      t.m10() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
      t.m11() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
      t.m12() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
      t.m13() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
      a0 = m20();
      a1 = m21();
      a2 = m22();
      a3 = m23();
      t.m20() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
      t.m21() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
      t.m22() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
      t.m23() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
      a0 = m30();
      a1 = m31();
      a2 = m32();
      a3 = m33();
      t.m30() = a0 * n.m00() + a1 * n.m10() + a2 * n.m20() + a3 * n.m30();
      t.m31() = a0 * n.m01() + a1 * n.m11() + a2 * n.m21() + a3 * n.m31();
      t.m32() = a0 * n.m02() + a1 * n.m12() + a2 * n.m22() + a3 * n.m32();
      t.m33() = a0 * n.m03() + a1 * n.m13() + a2 * n.m23() + a3 * n.m33();
	   return t;
   }

   /**
    * Matrix multiplication.  Multiplies the current matrix by the matrix n
    * ( m = m n ) and stores the result in m.
    * @param   n  Matrix to multiply the current matrix by
    * @return  Returns the address of the current matrix.
    */
   Matrix4x4& operator *= (const Matrix4x4& n)
   {
      *this = *this * n;
      return *this;
   }

   /**
    * Matrix multiplication by a scalar.  Multiplies the current matrix 
    * by the scalar s.
    * @param  s   Scalar to multiply the current matrix by
    * @return     Returns the address of the current matrix.
    */
   Matrix4x4 & operator *= (const float s)
   {
      for (int r = 0; r < 16; r++)
		   a[r] *= s;

	   return *this;
   }

   /**
    * Transforms a coordinate by the matrix.
    * @param   v  Homogeneous point.
    * @return  Returns the transformed homogeneous coordinate position.
    */
   HPoint3 operator *(const HPoint3& v) const
   {
      return HPoint3((a[0] * v.x + a[4] * v.y + a[8]  * v.z + a[12] * v.w),
                     (a[1] * v.x + a[5] * v.y + a[9]  * v.z + a[13] * v.w),
		               (a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14] * v.w),
		               (a[3] * v.x + a[7] * v.y + a[11] * v.z + a[15] * v.w));
   }

   /**
    * Transforms a coordinate by the matrix.  Returns a homogeneous
    * coordinate position.  Assumes the w coordinate is 1.
    * @param   v  3D point to transform.
    * @return  Returns the transformed point.
    */
   HPoint3 operator *(const Point3& v) const
   {
      return HPoint3((a[0] * v.x + a[4] * v.y + a[8]  * v.z + a[12]),
                     (a[1] * v.x + a[5] * v.y + a[9]  * v.z + a[13]),
                     (a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14]),
                     1.0f);
   }

   /**
    * Transforms a vector (normal or direction) by the matrix. 
    * Only the upper 3x3 portion of the matrix is used (no translation).
    * @param   v  3D vector to transform.
    * @return  Returns the transformed direction.
    */
   Vector3 operator *(const Vector3& v) const
   {
      return Vector3((a[0] * v.x + a[4] * v.y + a[8]  * v.z),
                     (a[1] * v.x + a[5] * v.y + a[9]  * v.z),
                     (a[2] * v.x + a[6] * v.y + a[10] * v.z));
   }

   /**
    * Transforms a ray by the matrix.  Transforms the ray origin and
    * ray direction.
    * @param   ray   Ray to transform
    * @return  Returns the transformed ray.
    */
   Ray3 operator *(const Ray3& ray) const
   {
      return Ray3(*this * ray.o, *this * ray.d, true);
   }

   /**
    * Transposes the current matrix.
    * @return   Returns the address of the current matrix.
    */
   Matrix4x4& Transpose(void)
   {
      *this = GetTranspose();
      return *this;
   }

   /**
    * Calculates the transpose of the current 4x4 matrix and returns it.
    * The current matrix is unchanged.
    * @return   Returns the transpose of the current matrix.
    */
   Matrix4x4 GetTranspose() const
   {
	   Matrix4x4 t;
      t.m00() = m00();
      t.m01() = m10();
      t.m02() = m20();
      t.m03() = m30();
      t.m10() = m01();
      t.m11() = m11();
      t.m12() = m21();
      t.m13() = m31();
      t.m20() = m02();
      t.m21() = m12();
      t.m22() = m22();
      t.m23() = m32();
      t.m30() = m03();
      t.m31() = m13();
      t.m32() = m23();
      t.m33() = m33();
      return t;
   }

   // The following convenience methods allow creation of a composite 
   // modeling transfomation. Each method postmultiplies the current
   // matrix (similar to OpenGL)

   /**
    * Applies a translation to the current transformation matrix.
    * Instead of doing a standard matrix multiplication call, we just reset
    * the elements of the matrix that will change due to the translation.
	 * @param	x	   x translation
	 * @param	y	   y translation
	 *	@param	z	   Z translation
	 */
	void Translate(const float x, const float y, const float z)
   {
      // Set the matrix elements that change with a translate transform.
      m03() += m00() * x + m01() * y + m02() * z;
	   m13() += m10() * x + m11() * y + m12() * z;
	   m23() += m20() * x + m21() * y + m22() * z;
	   m33() += m30() * x + m31() * y + m32() * z;
   }

   /**
    * Applies a scaling to the current transformation matrix.
	 * @param	x	   x scaling
	 * @param	y	   y scaling
	 *	@param	z	   Z scaling
	 */
	void Scale(const float x, const float y, const float z)
   {
     	// Set the matrix elements that change with a scaling transform.
      m00() *= x;
      m10() *= x;	
	   m20() *= x;	
      m30() *= x;
      m01() *= y;	
      m11() *= y;	
      m02() *= z;
      m21() *= y;	
      m31() *= y;	
      m12() *= z;
      m22() *= z;	
      m32() *= z;
   }

   /**
    * Performs a counterclockwise rotation about the specified axis. 
    * @param   angle    Angle (degrees) for the rotation.
    * @param   x        x coordinate of the axis of rotation
    * @param   y        y coordinate of the axis of rotation
    * @param   z        z coordinate of the axis of rotation
    */
   void Rotate(const float angle, const float x, const float y, 
               const float z)
   {
      // Handle simple cases of rotation about a single axis
      if (x > 0.0f && y == 0.0f && z == 0.0f)
      {
         RotateX(angle);
         return;
      }
      else if (y > 0.0f && x == 0.0f && z == 0.0f)
      {
         RotateY(angle);
         return;
      }
      else if (z > 0.0f && x == 0.0f && y == 0.0f)
      {
         RotateZ(angle);
         return;
      }
  
      // Set up the standard rotation using quaternions

      // Scalar part
      float s = (float)cos(degreesToRadians(angle * 0.5f));

      // Vector part: a normalized direction vector
      Vector3 v(x, y, z);
      v.Normalize();
      v *= (float)sin(degreesToRadians(angle * 0.5f));
      float a = v.x;
      float b = v.y;
      float c = v.z;

      // Set the composite rotation matrix
      Matrix4x4 r;
      r.m00() = 1.0f - 2.0f * b * b - 2.0f * c * c;
      r.m01() = 2.0f * a * b - 2.0f * s * c;
      r.m02() = 2.0f * a * c + 2.0f * s * b;
      r.m10() = 2.0f * a * b + 2.0f * s * c;
      r.m11() = 1.0f - 2.0f * a * a - 2.0f * c * c;
      r.m12() = 2.0f * b * c - 2.0f * s * a;
      r.m20() = 2.0f * a * c - 2.0f * s * b;
      r.m21() = 2.0f * b * c + 2.0f * s * a;
      r.m22() = 1.0f - 2.0f * a * a - 2.0f * b * b;
      r.m33() = 1.0f;
      
      // Postmultiply the current matrix
      *this *= r;

   }

   /**
    * Performs a counterclockwise rotation about the x axis. 
    * @param   angle    Angle (degrees) for the rotation.
    */
   void RotateX(const float angle)
   {
     // The rotation matrix
      Matrix4x4 r;
      float radians = degreesToRadians(angle);
	   float cosa = cosf(radians);
	   float sina = sinf(radians);
	   r.m11() = cosa;		r.m12() = -sina;
	   r.m21() = sina;		r.m22() = cosa;

      // Postmultiply the current matrix
      *this *= r;
   }

   /**
    * Performs a counterclockwise rotation about the x axis. 
    * @param   angle    Angle (degrees) for the rotation.
    */
   void RotateY(const float angle)
   {
     // The rotation matrix
      Matrix4x4 r;
      float radians = degreesToRadians(angle);
	   float cosa = cosf(radians);
	   float sina = sinf(radians);
      r.m00() =  cosa;	r.m02() = sina;
	   r.m20() = -sina;	r.m22() = cosa;

      // Postmultiply the current matrix
      *this *= r;
   }

   /**
    * Performs a counterclockwise rotation about the x axis. 
    * @param   angle    Angle (degrees) for the rotation.
    */
   void RotateZ(const float angle)
   {
     // The rotation matrix
      Matrix4x4 r;
      float radians = degreesToRadians(angle);
	   float cosa = cosf(radians);
	   float sina = sinf(radians);
	   r.m00() = cosa;		r.m01()  = -sina;
	   r.m10() = sina;		r.m11()  =  cosa;

      // Postmultiply the current matrix
      *this *= r;
   }

   /**
    * Inverts this matrix
    */
   void Invert()
   {
      *this = GetInverse();
   }

   /**
    * Calculates the inverse of the current 4x4 matrix and returns it.
    * @return  Returns the inverse of the current matrix.
    */
   Matrix4x4 GetInverse() const
   {
	   int j, k;
	   int ind;
	   float v1, v2;
	   Matrix4x4 t = *this;
      Matrix4x4 b;
	   for (int i = 0; i < 4; i++)
	   {
         // Find pivot
		   v1 = t.m(i, i);
		   ind = i;
		   for (j = i + 1; j < 4; j++)
		   {
			   if (fabs(t.m(j, i)) > fabs(v1))
			   {
				   ind = j;
				   v1 = t.m(j, i);
			   }
		   }

         // Swap columns
		   if (ind != i)
		   {
			   for (j = 0; j < 4; j++)
			   {
				   v2 = b.m(i, j);
				   b.m(i, j) = b.m(ind, j);
				   b.m(ind, j) = v2;
				   v2 = t.m(i, j);
				   t.m(i, j) = t.m(ind, j);
				   t.m(ind, j) = v2;
			   }
		   }

		   // The matrix is singular (has no inverse), set the inverse
		   //    to the identity matrix.
		   if (v1 == 0.0f)
		   {
            extern void logmsg(const char *message, ...);
			   logmsg("InvertMatrix: Singular matrix");
            b.SetIdentity();
			   return b;
		   }

		   for (j = 0; j < 4; j++)
		   {
			   t.m(i, j) /= v1;
			   b.m(i, j) /= v1;
		   }

         // Eliminate column
		   for (j = 0; j < 4; j++)
		   {
 			   if (j == i)
				   continue;

			   v1 = t.m(j, i);
			   for (k = 0; k < 4; k++)
			   {
				   t.m(j, k) -= t.m(i, k) * v1;
				   b.m(j, k) -= b.m(i, k) * v1;
			   }
		   }
	   }



	   return b;
   }

   /**
    * Logs a message followed by the matrix.
    * @param   str   String to print to log file
    */  
   void Log(const char* str) const
   {
      extern void logmsg(const char *message, ...);
	   logmsg("  %s", str);
	   logmsg("%.3f %.3f %.3f %.3f", m00(), m01(), m02(), m03());
	   logmsg("%.3f %.3f %.3f %.3f", m10(), m11(), m12(), m13());
	   logmsg("%.3f %.3f %.3f %.3f", m20(), m21(), m22(), m23());
	   logmsg("%.3f %.3f %.3f %.3f", m30(), m31(), m32(), m33());
   }

private:
   float a[16];
};

#endif
