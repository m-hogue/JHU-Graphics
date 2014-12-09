//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt

//	File:    SphereSection.h
//	Purpose: Scene graph geometry node representing a section of a 
//          sphere.
//
//============================================================================

#ifndef __SPHERESECTION_H
#define __SPHERESECTION_H

#include "geometry/geometry.h"

class SphereSection : public TriSurface
{
public:
	/**
    * Creates a section of a sphere with bounds given by minLat, maxLat and
    * minLng, maxLng. The number of subdivisions of each is also given. Can be 
    * used to construct a full sphere. This method uses a single vertex at the
    * north and south pole and avoids creating extra triangles for polar regions.
    * @param   minLat   Minimum latitude
    * @param   maxLat   Maximum latitude
    * @param   nLat     Number of divisions of latitude
    * @param   minLng   Minimum longitude
    * @param   maxLng   Maximum longitude
    * @param   nLng     Number of divisions of longitude
    * @param   radius   Radius of the sphere 
    */
	SphereSection(const float minLat, const float maxLat, const unsigned int nLat,
                 const float minLng, const float maxLng, const unsigned int nLng,
                 const float radius, const int positionLoc, const int normalLoc,
				 const int textureLoc)
	{
      // Convert to radians
      float minLatRadians = degreesToRadians(minLat);
      float maxLatRadians = degreesToRadians(maxLat);
      float minLngRadians = degreesToRadians(minLng);
      float maxLngRadians = degreesToRadians(maxLng);

      // Create a vertex list with unit length normals
      float cosLat, cosLng, sinLng;
		float dLat = (maxLatRadians - minLatRadians) / (float)nLat;
      float dLng = (maxLngRadians - minLngRadians) / (float)nLng;
      VertexAndNormal vtx;
		for (float lng = minLngRadians; lng < maxLngRadians - dLng + EPSILON; lng += dLng)
      {
         cosLng = cosf(lng);
         sinLng = sinf(lng);
         for (float lat = minLatRadians; lat <= maxLatRadians + EPSILON; lat += dLat)
         {
            cosLat = cosf(lat);
            vtx.m_normal.x = cosLng * cosLat;
            vtx.m_normal.y = sinLng * cosLat;
            vtx.m_normal.z = sinf(lat);
			   vtx.m_vertex.x = radius * vtx.m_normal.x;
            vtx.m_vertex.y = radius * vtx.m_normal.y;
            vtx.m_vertex.z = radius * vtx.m_normal.z;
	         m_vertexList.push_back(vtx);
			 m_textureList.push_back(Vector2(lat, lng));
         }
      }

      // Copy the first column of vertices (eliminates chances of roundoff)
      for (unsigned int i = 0; i <= nLat + 1; i++)
         m_vertexList.push_back(m_vertexList[i]);

      // Construct face list.  There are nLat+1 rows and nLng+1 columns. Create VBOs
      ConstructRowColFaceList(nLat + 1, nLng+1);
      CreateVertexBuffers(positionLoc, normalLoc, textureLoc);
	}
	
private:
	// Make default constructor private to force use of the constructor
	// with number of subdivisions.
	SphereSection() { };
};

#endif