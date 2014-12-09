//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    BallTransformNode.h
//	Purpose:	Transform node that controls the movement of a ball within
//          a confined space.
//
//============================================================================

#ifndef __BALLTRANSFORMNODE_H
#define __BALLTRANSFORMNODE_H

#include "Scene/Scene.h"
#include <list>

/**
 * Ball transform node.
 */
class BallTransform: public TransformNode
{
public:
   /**
    * Constructor given the number of frames per second.
    */
   BallTransform(const float fps)
   {
	   m_fps = fps;
      // Set a random initial position with x,y values between 
      // -40 and 40 and z between 25 and 75
      //m_position.Set(getRandom(-40.0f, 40.0f), getRandom(-40.0f, 40.0f),
      //               getRandom(25.0f, 75.0f));

      // Set a random radius between 3 and 7
      //m_radius = getRandom(3.0f, 7.0f);

      // Set a random initial direction
      //m_direction.Set(getRand01(), getRand01(), getRand01());
      //m_direction.Normalize();
      
      // Set a random speed between 5 and 15 units per second. Convert it to 
      // speed per frame.
      //m_speed = getRandom(5.0f, 15.0f) / fps;
      //setTransform();
   }

   /**
    * Destructor
    */
	virtual ~BallTransform() { }

   /**
	 * Update the scene node and its children
	 */
   virtual void Update(SceneState& sceneState)
   {
	   //fprintf(stderr, "\nball update:\n");
	   //fprintf(stderr, "\tposition(%f,%f,%f)\n", m_position.x, m_position.y, m_position.z);
	   //fprintf(stderr, "\tspeed: %f\n", m_speed);
	   //fprintf(stderr, "\tfps: %f\n", m_fps);
	   //fprintf(stderr, "\tdirection(%f,%f,%f)\n", m_direction.x, m_direction.y, m_direction.z);
	   //fprintf(stderr, "\tradius: %f\n\n", m_radius);
      // Check if intersection occured
      if (m_intersectTime != 0.0f && m_intersectTime < 1.0f)
      {
         // An intersect occured move along the current direction by parameter t, 
         // reflect the direction about the normal to the intersected plane, then move 
         // the position along the new direction by the remaining distance.
         m_position  = m_position + m_direction * (m_speed * m_intersectTime);
         m_direction = m_direction.Reflect(m_intersectPlane.GetNormal());
         m_position  = m_position + m_direction * (m_speed * (1.0f - m_intersectTime));
      }
      else
      {
         // No intersection - move origin along direction vector
         m_position = m_position + (m_direction * m_speed);
      }

      // Update this node's transformations
      setTransform();       

      // Update all children
		SceneNode::Update(sceneState);
   }

   // Use the base class Draw method

   /**
    * Set current position of ball - normally just set when ball is constructed.
	*/
   void SetPosition(const Point3 &position) {
	   m_position = position;
   }

   /**
    * Get the current position
    * @return  Returns the current posiiton / center of the sphere.
    */
   const Point3& GetPosition() const
   {
      return m_position;
   }

   /**
    * sets the radius
	*/
   void SetRadius(const float radius) {
	   m_radius = radius;
   }

   /**
    * Gets the ball radius
    * @return   Returns the radius.
    */
   float GetRadius() const
   {
      return m_radius;
   }

   /**
    * set direction
	*/
   void SetDirection(const Vector3 &direction) {
	   m_direction = direction;
	   m_direction.Normalize();
   }

   /**
    * Get the direction of travel (a unit vector).
    * @return  Returns the direction of travel.
    */
   const Vector3& GetDirection() const
   {
      return m_direction;
   }

   void SetSpeed(const float speed) {
	   m_speed = speed/m_fps;
   }
   /**
    * Gets the speed of travel.
    * @return   Returns the speed of travel
    */
   float GetSpeed() const
   {
      return m_speed;
   }

   /**
    * Special method to set intersect time (for simple collision detection)
    * @param  t  Time until intersect (0.0 - no intersect)
    */
   void SetIntersectTime(const float t)
   {
      m_intersectTime = t;
   }

   /**
    * Get the time until nearest intersection
    * @return  Returns the time to nearest intersection (0.0 if none occurs)
    */
   float GetIntersectTime() const
   {
      return m_intersectTime;
   }

   /**
    * Set the plane of intersection (use this in collision response)
    * @param  plane  Plane of intersection
    */
   void SetIntersectPlane(const Plane& plane)
   {
      m_intersectPlane = plane;
   }

   /**
    * Intersection the ray with the plane. Use a radius r to indicate a 
    * sphere. The ray indicates the movement of the sphere. The return 
    * value indicates the time along the ray where an intersect of a 
    * sphere moving along the ray. A return value > 1.0 indicates no
    * intersection occurs.
    * @plane  plane  Plane to test intersection against
    */
   float IntersectWithPlane(Plane& plane)
   {
      // Find the signed distance of sphere at start and end of the 
      // time interval. Note that speed indicates the distance moved
      // per frame
      float dc = plane.Solve(m_position);
      float de = plane.Solve(m_position + m_direction * m_speed);

      // No intersect if both dc and de are > r
      if (dc > m_radius && de > m_radius)
         return 100.0f;

      // Intersect occurs when sphere first touches plane
      return (dc - m_radius) / (dc - de);
   }

   /**
    * Test if this moving ball intersect another moving ball
    * @param  ball  Other moving ball to test intersection against
    */
   bool IntersectBall(BallTransform* ball)
   {
      // Test if 2 balls already are intersecting. This can occur if an intersection is missed in a
      // prior frame due to not checking all possible intersections.
      // Since ray-sphere intersection does not return a value < 0 it will not detect this case.
      Vector3 vb = ball->GetPosition() - GetPosition();
      float vbn = vb.Norm();
      if (vbn < (ball->GetRadius() + m_radius))
      {
         // Set the time of intersection at 0 and the plane of intersection (only need the 
         // normal so a reflection can occur)
         vb *= (1.0f / vbn);                        // Normalize vb      
         ball->SetIntersectTime(0.0f);
         ball->SetIntersectPlane(Plane(ball->GetPosition(), vb));
         SetIntersectTime(0.0f);
         SetIntersectPlane(Plane(GetPosition(), vb * -1.0f));
         return true;
      }

      // Create a ray at the position of this ball with direction vector = difference of the 2 velocity vectors.
      // Get the length of v (note that the ray to sphere intersection requires a unit length ray direction)
      Vector3 v = m_direction * m_speed - (ball->GetDirection() * ball->GetSpeed());
      float l   = v.Norm();
      Ray3 ray(m_position, v * (1.0f / l));

      // Construct a bounding sphere at the center of the other ball with radius equal to
      // sum of the 2 balls
      BoundingSphere sphere(ball->GetPosition(), ball->GetRadius() + m_radius);

      float t = ray.Intersect(sphere);
      if (t > EPSILON && t < l + EPSILON)
      {
         // Find the centers of both balls when intersection occurs - convert t into proper units
         t *= 1.0f / l;
         Point3 c1 = m_position + (m_direction * (m_speed * t));
         Point3 c2 = ball->GetPosition() + (ball->GetDirection() * (ball->GetSpeed() * t));

         // Find the intersect point - it is along a vector between the 2 sphere centers
         Vector3 d = (c2 - c1).Normalize();
         Point3 intPt = c1 + d * m_radius;

         // The plane of intersection is at the intersection point with normal along the vector between
         // the centers (normal will be opposite direction for each ball)
        
         // Set the time of intersection and the plane of intersection for both balls
         ball->SetIntersectTime(t);
         ball->SetIntersectPlane(Plane(intPt, d));
         SetIntersectTime(t);
         SetIntersectPlane(Plane(intPt, d * -1.0f));
         return true;
      }
      else
         return false;
   }

   // Sets the transformation matrix
   void setTransform()
   {
	   m_matrix.SetIdentity();
	   m_matrix.Translate(m_position.x, m_position.y, m_position.z);
	   m_matrix.Scale(m_radius, m_radius, m_radius);
   }

protected:
   float     m_radius;            // Radius
   float     m_speed;             // Speed - units per frame
   Point3    m_position;          // Current position
   Vector3   m_direction;         // Direction vector (unit length)
   float	 m_fps;				  // frames per second

   // Plane where an intersection occurs
   Plane     m_intersectPlane;

   // Time of interesection (0.0 if no intersection occurs)
   float     m_intersectTime;

   // Set the default constructor to private to force use of the 
   // one with arguments
   BallTransform() {}

   

   // Create a random value between a specified minv and maxv.
   float getRandom(const float minv, const float maxv)
   {
      return minv + ((maxv - minv) * (float)rand() / (float)RAND_MAX);
   } 

   float getRand01() const
   {
      return (float)rand() / (float)RAND_MAX;
   }
};

#endif