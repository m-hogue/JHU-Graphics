//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSphere.h
//	Purpose:	Simple geometry node that defines a unit sphere and draws
//          it with glutSolidSphere.
//
//============================================================================

#ifndef __UNITSPHERE_H
#define __UNITSPHERE_H

#include <vector>
#include "scene/scene.h"

/**
 * Unit sphere geometry node.
 */
class UnitSphere: public GeometryNode
{
public:
   /**
    * Constructor.
    */
	UnitSphere(const unsigned int nLat, const unsigned int nLng, const int positionLoc, const int normalLoc)
	{
      // Convert to radians
      float minLatRadians = degreesToRadians(-90.0f);
      float maxLatRadians = degreesToRadians( 90.0f);
      float minLngRadians = degreesToRadians(-180.0f);
      float maxLngRadians = degreesToRadians( 180.0f);

      // Compute delta lat,lng
      float dLat = (maxLatRadians - minLatRadians) / (float)nLat;
      float dLng = (maxLngRadians - minLngRadians) / (float)nLng;

      // Create vertex list. Store vertex and normal
	   std::vector<VertexAndNormal> vertexList;
      float cosLat1, sinLat1, cosLat2, sinLat2, cosLng, sinLng;
      VertexAndNormal vtx;
		for (float lat1 = minLatRadians, lat2 = lat1 + dLat; lat2 <= maxLatRadians + EPSILON; lat1 = lat2, lat2 += dLat)
      {
         cosLat1 = cosf(lat1);
         sinLat1 = sinf(lat1);
         cosLat2 = cosf(lat2);
         sinLat2 = sinf(lat2);

         // Iterate over longitude, connecting a vertex on hte next latitude row with one
         // on the current latitude row. Since we wrap around to the same longitude we do
         // not need to repeat vertices to create degenerate triangles to connect each row.
         for (float lng = minLngRadians; lng <= maxLngRadians + EPSILON; lng += dLng)
         {
            cosLng = cosf(lng);
            sinLng = sinf(lng);

            // Add a vertex on the upper latitude
            vtx.m_vertex.Set(cosLng * cosLat2, sinLng * cosLat2, sinLat2);
			   vtx.m_normal = vtx.m_vertex;
	         vertexList.push_back(vtx);

            // Add a vertex on the lower latitude
            vtx.m_vertex.Set(cosLng * cosLat1, sinLng * cosLat1, sinLat1);
			   vtx.m_normal = vtx.m_vertex;
	         vertexList.push_back(vtx);
         }
      }
      m_vertexCount = vertexList.size();

      // Create a buffer object and load the data
      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexAndNormal), 
               (GLvoid*)&vertexList[0].m_vertex.x, GL_STATIC_DRAW);
	
      // Allocate a VAO, enable it
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

      // Bind the VBO, set vertex attribute pointers for position and normal (using stride and offset).
      // Enable the arrays.
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (void*)0);
      glEnableVertexAttribArray(positionLoc);
      glVertexAttribPointer(normalLoc,   3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (void*)(sizeof(Point3)));
      glEnableVertexAttribArray(normalLoc);

      // Make sure changes to this VAO are local
      glBindVertexArray(0);
   }
	
   /**
    * Destructor
    */
	virtual ~UnitSphere() { }

	/**
	 * Draw the ball / sphere. Note that geometry nodes are at the
    * leaves of the tree, so no children exist.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      glBindVertexArray(m_vao);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
      glBindVertexArray(0);
	}

protected:
   GLuint m_vao;                 // Vertex Array Object
   GLuint m_vbo;                 // Vertex Buffer Object
   unsigned int m_vertexCount;   // Number of vertices in the sphere
};

#endif