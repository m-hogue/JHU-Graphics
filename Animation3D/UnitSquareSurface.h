//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSquareSurface.h
//	Purpose:	Simple geometry node that defines a unit square.
//
//============================================================================

#ifndef __UNITSQUARESURFACE_H
#define __UNITSQUARESURFACE_H

#include <vector>
#include "scene/scene.h"

/**
 * Unit square geometry node.
 */
class UnitSquareSurface1: public GeometryNode
{
public:
   /**
    * Constructor. Construct the vertex list for a triangle fan
    * representing a unit square. Make sure the vertices alternate
    * top to bottom in y
    */
	UnitSquareSurface1(const int positionLoc, const int normalLoc) 
   {
	   std::vector<VertexAndNormal> vertexList;
      VertexAndNormal vtx;
      vtx.m_normal.x = 0.0f;
      vtx.m_normal.y = 0.0f;
      vtx.m_normal.z = 1.0f;
	   vtx.m_vertex.x = -0.5f;
      vtx.m_vertex.y = 0.5f;
      vtx.m_vertex.z = 0.0f;
      vertexList.push_back(vtx);
      vtx.m_vertex.x = -0.5f;
      vtx.m_vertex.y = -0.5f;
      vertexList.push_back(vtx);
      vtx.m_vertex.x = 0.5f;
      vtx.m_vertex.y = 0.5f;
      vertexList.push_back(vtx);
      vtx.m_vertex.x = 0.5f;
      vtx.m_vertex.y = -0.5f;
      vertexList.push_back(vtx);
      m_vertexCount = vertexList.size();

      // Create a buffer object and load the data
      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexAndNormal), 
               (GLvoid*)&vertexList[0].m_vertex.x, GL_STATIC_DRAW);
	
      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
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
	virtual ~UnitSquareSurface1() { }

	/**
	 * Draw a unit square. Note that geometry nodes are at the
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
   unsigned int m_vertexCount;   // Number of vertices in the square
};

#endif