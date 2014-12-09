//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PointNode.h
//	Purpose:	Simple geometry node that draws points.
//
//============================================================================

#ifndef __POINTNODE_H
#define __POINTNODE_H

#include <vector>
#include "scene/scene.h"

/**
 * Unit sphere geometry node.
 */
class PointNode: public GeometryNode
{
public:
   /**
    * Constructor.
    * @param  color  Color for points
    */
	PointNode(const Color4& color)
	{
      m_color = color;

      // Create a buffer object and a vertex array object
      glGenBuffers(1, &m_vbo);
      glGenVertexArrays(1, &m_vao);
	}
	
   /**
    * Destructor
    */
	virtual ~PointNode()
   {
      glDeleteBuffers(1, &m_vbo);
      glDeleteVertexArrays(1, &m_vao);
   }

   /**
    * Adds a point to the list
    * @param  x   X screen location
    * @param  y   Y screen location
    */
   virtual void Add(const float x, const float y, int positionLoc)
   {
      m_vertexList.push_back(Point2(x,y));

      // Add the points to the buffer
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_vertexList.size() * sizeof(Point2), (GLvoid*)&m_vertexList[0], GL_DYNAMIC_DRAW);

      // Update the VAO
      glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glEnableVertexAttribArray(positionLoc);
      glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   }

	/**
	 * Draw the set of points.
    * @param  sceneState  Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      if (m_vertexList.size() > 0)
      {
         // Set the color
         glUniform4f(sceneState.m_colorLoc, m_color.r, m_color.g, m_color.b, m_color.a);
         
         // Bind the VAO and draw the lines
         glBindVertexArray(m_vao);
		   glDrawArrays(GL_POINTS, 0, m_vertexList.size());
         glBindVertexArray(0);

         checkError("End of Points:");
      }
	}

protected:
   GLuint m_vbo;                       // VBO
   GLuint m_vao;                       // Vertex Array Object
   Color4 m_color;                     // Color to draw the points
	std::vector<Point2> m_vertexList;   // Vertex list
};

#endif