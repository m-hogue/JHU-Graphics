//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TorusSurface.h
//	Purpose: Scene graph geometry node representing a torus.
//
//============================================================================

#ifndef __TORUSSURFACE_H
#define __TORUSSURFACE_H

#include <list>
#include "geometry/geometry.h"

class TorusSurface : public TriSurface
{
public:
	/**
    * Creates a torus with two radii specified: the ring radius and the radius
    * of the swept circle (tube).  The number of divisions around the ring and
    * the number of divisions around the tube are specified.  Scaling of the
    * torus can be performed but will it will scale both the ring and tube
    * radius.
    * @param   ringradius  Radius of the ring
    * @param   tuberadius  Radius of the circle swept about the ring
    * @param   nring       Number of divisions around the ring
    * @param   ntube       Number of divisions around the tube
    */
	TorusSurface(const float ringradius, const float tuberadius, 
                const int nring, const int ntube, const int positionLoc,
                const int normalLoc, const int textureLoc)
	{
      // Use <= so we wrap around to make the last vertices meet the first
      int i, j;
	   float v, phi, theta; 
	   float dphi   = (2.0f * (float)M_PI) / (float)ntube;
	   float dtheta = (2.0f * (float)M_PI) / (float)nring;
      Vector3 tan1;                       // Tangent vector with respect to ring
      Vector3 tan2;                       // Tangent vector with respect to tube
      VertexAndNormal vtx;
	   for (theta = 0.0f, i = 0; i <= nring; i++, theta += dtheta)
	   {
		   for (phi = 0.0f, j = 0; j <= ntube; j++, phi += dphi)
		   {
            // Compute vertex
            v = (ringradius + tuberadius * cos(phi));
            vtx.m_vertex.Set(v * cosf(theta), v * sinf(theta), tuberadius * sinf(phi));
			
            // Compute normal. It is the cross product of the two tangents (one with respect to the ring 
            // rotation and one with repect to the tube rotation). These are found by taking the derivative
            // of the parametric equation with respect to theta and phi.
            tan1.Set(-sinf(theta), cosf(theta), 0.0f); 
            tan2.Set(cosf(theta) * (-sinf(phi)), sinf(theta) * (-sinf(phi)), cosf(phi));
            vtx.m_normal = (tan1.Cross(tan2)).Normalize();
            m_vertexList.push_back(vtx);
			m_textureList.push_back(Vector2(acosf(vtx.m_vertex.x / v) * (2.0f * (float)M_PI), 
										    acosf(vtx.m_vertex.z / ringradius) / (2.0f * (float)M_PI)));
		   }
	   }

      // Construct face list.  There are ntube+1 rows and nring+1 columns. Create VBOs
      ConstructRowColFaceList(ntube+1, nring+1);
      CreateVertexBuffers(positionLoc, normalLoc, textureLoc);
	}
	
private:

	// Make default constructor private to force use of the constructor
	// with arguments.
	TorusSurface() { };
};

#endif