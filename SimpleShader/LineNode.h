//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//


//	Author:	David W. Nesbitt
//	File:    LineNode.h
//	Purpose:	Simple geometry node that draws a connected line.
//
//============================================================================

#ifndef __LINENODE_H
#define __LINENODE_H

#include <vector>
#include "Scene/Scene.h"

/**
 * Unit sphere geometry node.
 */
class LineNode: public GeometryNode
{
public:
   /**
    * Constructor.
    */
	LineNode(std::vector<Point2> pts, const Color4& color, const int positionLoc)
	{
      // Copy the color and set the number of vertices
      m_color = color;
      m_vertexCount = pts.size();

      // Create vertex buffer object
      glGenBuffers(1, &m_vbo);

      // Add the points to the buffer
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Point2),  (GLvoid*)&pts[0], GL_STATIC_DRAW);

      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glEnableVertexAttribArray(positionLoc);
      glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	
   /**
    * Destructor
    */
	virtual ~LineNode() 
   {
      glDeleteBuffers(1, &m_vbo);
      glDeleteVertexArrays(1, &m_vao);
   }

	/**
	 * Draw the lines
    * @param  sceneState  Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      // Set the color
      glUniform4f(sceneState.m_colorLoc, m_color.r, m_color.g, m_color.b, m_color.a);

      // Bind the VAO and draw the lines
      glBindVertexArray(m_vao);
		glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
      glBindVertexArray(0);
	}

protected:
   unsigned int m_vertexCount;         // Number of vertices
   GLuint m_vbo;                       // VBO for positions
   GLuint m_vao;                       // Vertex Array Object
   Color4 m_color;                     // Color to draw the lines
};

#endif