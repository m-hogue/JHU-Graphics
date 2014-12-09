//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.461 Principles of Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    VectorTest.cpp
//	Purpose: Test the vector library.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>

#include "geometry/geometry.h"
#include <math.h>

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("MatrixTest.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}


int main(int argc, char* argv[])
{
   logmsg("Matrix and Plane Test");

   Point3 v0(5, -3, 5);
   Point3 v1(5, -5, -1);
   Point3 v2(7, -4, -3);
   Plane p(v0, v1, v2);

   Point3 E(7, -3, 2);

   float t = p.Solve(E);
   logmsg("t value: %f", t);

   Vector3 n = p.GetNormal();
   Vector3 w = Vector3(v0, E);
   // compute projection of w onto n
   float d = abs(w.Dot(n)) / abs(n.Norm());
   logmsg("distance: %f", d);

   
   
   

   /************ Planes *************/

   //// Construct 3 points (not all in x,y plane!)
   //Point3 m(-5.0f, -3.0f,  5.0f);
   //Point3 n( 3.0f, -5.0f,  1.0f);
   //Point3 o( 4.0f,  4.0f, -3.0f);
   //logmsg("\nPlane Test\n");
   //logmsg("Point m (%.2f, %.2f, %.2f)", m.x, m.y, m.z);
   //logmsg("Point n (%.2f, %.2f, %.2f)", n.x, n.y, n.z);
   //logmsg("Point o (%.2f, %.2f, %.2f)", o.x, o.y, o.z);

   //// Get the cross product of vector mn and mo
   //Vector3 perp = (n-m).Cross(o-m);
   //logmsg("Cross Product of mn and mo = %.2f %.2f %.2f", perp.x, perp.y, perp.z);

   //// Construct a plane through m,n,o
   //Plane plane(m, n, o);
   //logmsg("Plane through points m,n,o = %.2f %.2f %.2f %.2f", 
   //               plane.a, plane.b, plane.c, plane.d);

   //plane.Normalize();
   //logmsg("Normalized Plane = %.2f %.2f %.2f %.2f", plane.a, plane.b, plane.c, plane.d);

   //// Construct plane with point m and perp
   //Plane plane2(m, perp);
   //plane2.Normalize();
   //logmsg("Normalized Plane with point m and normal perp = %.2f %.2f %.2f %.2f", 
   //               plane.a, plane.b, plane.c, plane.d);

   //// Check if point n is on the plane
   //float s = plane.Solve(n);
   //if (s > EPSILON)
   //   logmsg("Point n is in front of the plane: s = %.2f", s);
   //else if (s < -EPSILON)
   //   logmsg("Point n is behind the plane: s = %.2f", s);
   //else  
   //   logmsg("Point n is on the plane");

   //// Check point k against the plane
   //Point3 k(5.0, 2.0, 7.0f);
   //logmsg("Point k (%.2f, %.2f, %.2f)", k.x, k.y, k.z);
   //s = plane.Solve(k);
   //if (s > EPSILON)
   //   logmsg("Point k is in front of the plane: s = %.2f", s);
   //else if (s < -EPSILON)
   //   logmsg("Point k is behind the plane: s = %.2f", s);
   //else  
   //   logmsg("Point k is on the plane");

   //// Construct a vector from point a3 to point b3
   //Point3 a3( 0.0f,  -2.0f,  0.0f);
   //Point3 b3(-4.0f,  2.0f,  5.0f);
   //Vector3 v3(a3, b3);
   //logmsg("Vector v3 (%.2f, %.2f, %.2f)", v3.x, v3.y, v3.z);
   //
   //// Reflect vector v3 off the plane
   //Vector3 r3 = v3.Reflect(plane.GetNormal());
   //logmsg("Reflection of v3 off the plane = %.2f, %.2f, %.2f", r3.x, r3.y, r3.z);

   //// Create a bounding sphere centered at (0,0,0) with radius 4.0f
   //logmsg("\nRay Tests\n");
   //Point3 origin(0.0f, 0.0f, 0.0f);
   //BoundingSphere sphere(origin, 4.0f);

   //// Create a unit length ray starting at (5, 0, 0) in the +x direction and test intersection
   //Ray3 ray1(Point3(5.0f, 0.0f, 0.0f), Point3(10.0f, 0.0f, 0.0f), true);
   //logmsg("Ray1 Direction: %f %f %f", ray1.d.x, ray1.d.y, ray1.d.z);
   //float t = ray1.Intersect(sphere);
   //if (t != 0.0f)
   //{
   //   Point3 intersect = ray1.Intersect(t);
   //   logmsg("   Ray1 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   //}
   //else
   //   logmsg("   Ray1 does not intersect sphere");

   //// Create a unit length ray starting at (-15, 10, 0) in the +x direction and test intersection
   //Ray3 ray2(Point3(-15.0f, 10.0f, 0.0f), Point3(10.0f, 10.0f, 0.0f), true);
   //logmsg("Ray2 Direction: %f %f %f", ray2.d.x, ray2.d.y, ray2.d.z);
   //t = ray2.Intersect(sphere);
   //if (t != 0.0f)
   //{
   //   Point3 intersect = ray2.Intersect(t);
   //   logmsg("   Ray2 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   //}
   //else
   //   logmsg("   Ray2 does not intersect sphere");

   //// Create a unit length ray starting at (-15, 2, 0) in the +x direction and test intersection
   //Ray3 ray3(Point3(-15.0f, 2.0f, 0.0f), Point3(10.0f, 2.0f, 0.0f), true);
   //logmsg("Ray3 Direction: %f %f %f", ray3.d.x, ray3.d.y, ray3.d.z);
   //t = ray3.Intersect(sphere);
   //if (t != 0.0f)
   //{
   //   Point3 intersect = ray3.Intersect(t);
   //   logmsg("   Ray3 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   //}
   //else
   //   logmsg("   Ray3 does not intersect sphere");

   //// Create a unit length ray starting at (2, 2, 0) in the +y direction and test intersection
   //Vector3 y(0.0f, 1.0f, 0.0f);
   //Ray3 ray4(Point3(2.0f, 2.0f, 0.0f), y);
   //logmsg("Ray4 Direction: %f %f %f", ray4.d.x, ray4.d.y, ray4.d.z);
   //t = ray4.Intersect(sphere);
   //if (t != 0.0f)
   //{
   //   Point3 intersect = ray4.Intersect(t);
   //   logmsg("   Ray4 intersects sphere at %f %f %f, t = %f", intersect.x, intersect.y, intersect.z, t);
   //}
   //else
   //   logmsg("   Ray4 does not intersect sphere");

   //logmsg("\nMatrix Tests\n");

   //Matrix4x4 I;
   //I.SetIdentity();
   //I.Log("Identity Matrix");
   //
   //// Set a scaling matrix
   //Matrix4x4 S;
   //S.m00() = 10.0f;	
   //S.m11() = 10.0f;
   //S.m22() = 10.0f;
   //S.Log("Scaling Matrix");

   //// Set a translation matrix
   //Matrix4x4 T;
   //T.m03() = -5.0f;	
   //T.m13() = 10.0f;
   //T.m23() = 15.0f;
   //T.Log("Translation Matrix");

   //// Set a rotation matrix about z
   //Matrix4x4 R;
   //R.m00() = cosf(degreesToRadians(45.0f));
   //R.m11() = R.m00();
   //R.m10() = sinf(degreesToRadians(45.0f));
   //R.m01() = -R.m10();
   //R.Log("Rotation (z) Matrix");

   //// Perform composite
   //Matrix4x4 C = T * R * S;
   //C.Log("Composite TRS");

   //// Transform point 1,1,1,1 (homogeneous point)
   //HPoint3 P(1.0f, 1.0f, 1.0f, 1.0f);
   //HPoint3 P1 = C * P;
   //logmsg("Transformed Point is %f %f %f %f", P1.x, P1.y, P1.z, P1.w);

   //// Show how the order matters
   //Matrix4x4 C2 = S * R * T;
   //C2.Log("Composite SRT");
   //HPoint3 P2 = C2 * P;
   //logmsg("Transformed Point is %f %f %f %f", P2.x, P2.y, P2.z, P2.w);

   //// Test the local Translate, Rotate, Scale methods. Should match C
   //logmsg("\nTest Local Translate, Rotate, Scale");
   //logmsg("Should match Composite TRS Above");
   //Matrix4x4 M;
   //M.Translate(-5.0f, 10.0f, 15.0f);
   //M.Rotate(45.f, 0.0f, 0.0f, 1.0f);
   //M.Scale(10.0f, 10.0f, 10.0f);
   //M.Log("After TRS: Composite matrix is");

   //// New composite matrix
   //C.SetIdentity();
   //C.Translate(-10.0f, 20.0f, 3.0f);
   //C.Rotate(60.f, 1.0f, 0.0f, 0.0f);     // 60 degrees about x axis
   //C.Scale(10.0f, 10.0f, 10.0f);
   //C.Log("New Composite TRS");

   //// Transform point 1,1,0
   //Point3 P3(1.0f, 1.0f, 0.0f);
   //P1 = C * P3;
   //logmsg("Transformed Point is %f %f %f %f", P1.x, P1.y, P1.z, P1.w);

   //// Transform the vector (1, 0, 0)
   //Vector3 normal(1.0f, 0.0f, 0.0f);
   //Vector3 transformedNormal = C * normal;
   //logmsg("Transformed Normal is %f %f %f", transformedNormal.x, transformedNormal.y, transformedNormal.z); 

   //// Get the transpose of C (this method leaves the contents of C unchanged)
   //Matrix4x4 D = C.GetTranspose();
   //D.Log("D = Transpose of C");

   //// Transpose C
   //C.Transpose();
   //C.Log("Transpose of C");

   //// Get the inverse of C
   //Matrix4x4 CInverse = C.GetInverse();
   //CInverse.Log("Inverse of C");

   //// Verify the Inverse
   //Matrix4x4 VerifyInverse = C * CInverse;
   //VerifyInverse.Log("Matrix times its Inverse (Should be Identity Matrix)");

   //// Test the ability to get the matrix and set it
   //logmsg("Verify Set and Get Methods: Result should match rotation matrix");
   //const float* mat = R.Get();
   //R.Set(mat);
   //R.Log("Rotation matrix after Get and Set");

   //// Transform a ray
   //Ray3 tr = R * ray1;
   //logmsg("Transformed Ray Origin is %f %f %f  Ray Direction = %f %f %f", tr.o.x, tr.o.y, tr.o.z, tr.d.x, tr.d.y, tr.d.z);

   return 1;
}