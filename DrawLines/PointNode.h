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

/**
 * Points. Maintain the VBO holding points to draw. Note that we do not keep a local
 * copy of the points.
 */
class PointNode: public GeometryNode
{
public:
   /**
    * Constructor.
    * @param  capacity   Maximum number of points
    * @param  pointSize  Point size
    */
	PointNode(const unsigned int capacity, const float pointSize, const int positionLoc)
	{
      m_pointSize = pointSize;
      m_vertexCount = 0;
      m_capacity    = capacity;

      // Create a buffer object
      glGenBuffers(1, &m_vbo);

      // Set up a buffer for dynamic draw
      std::vector<Point2> pts(capacity);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(Point2), (GLvoid*)&pts[0], GL_DYNAMIC_DRAW);

      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(positionLoc);
	}
	
   /**
    * Destructor
    */
   virtual ~PointNode() {}

   /**
    * Adds a list of points to the list
    * @param  pts  List of points
    */
   virtual void Add(const std::vector<Point2>& pts)
   {
      if ((pts.size() + m_vertexCount) >= m_capacity)
      {
         printf("Exceeding point capacity in Add (pts)\n");
         return;
      }

      // Update the vertex buffer object. Add just the new vertices
      // Set the offset based on number of previous vertices
      if (pts.size() > 0)
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
         glBufferSubData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Point2), pts.size() * sizeof(Point2),
                        (GLvoid*)&pts[0]);
         m_vertexCount += pts.size();
      }
   }

   /**
    * Adds a point to the list and update the VBO
    * @param  x   X screen location
    * @param  y   Y screen location
    */
   virtual void Add(const float x, const float y)
   {  
      if (m_vertexCount >= m_capacity - 1)
      {
         printf("Exceeding point capacity in Add (vertex)\n");
         return;
      }

      // Update the vertex buffer object. Add just the new vertex
      // Set the offset based on number of previous vertices
      Point2 pt(x,y);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Point2), sizeof(Point2), (GLvoid*)&pt);
      m_vertexCount++;
   }

   /**
    * Replaces the points in the VBO with the supplied list of points
    * @param  pts  List of points
    */
   virtual void Replace(const std::vector<Point2>& pts)
   {
      if (pts.size() >= m_capacity)
      {
         printf("Exceeding point capacity in Replace\n");
         return;
      }

      // Update the vertex buffer object. Add just the new vertices
      // Set the offset based on number of previous vertices
      if (pts.size() > 0)
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
         glBufferSubData(GL_ARRAY_BUFFER, 0, pts.size() * sizeof(Point2), (GLvoid*)&pts[0]);
      }
      m_vertexCount = pts.size();
   }

   /**
    * Clears the points so none get drawn (sets vertex count to 0);
    */
   void Clear()
   {
      m_vertexCount = 0;
   }

	/**
	 * Draw the set of points.
    * @param  sceneState  Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
   {
      if (m_vertexCount > 0)
      {
         // Set the point size within the point shader.
         glUniform1f(sceneState.m_pointSizeLoc, m_pointSize);
    
         // Bind the VBO, enable vertex attributes, set vertex attribute pointer, then draw the points
         glBindVertexArray(m_vao);
         glDrawArrays(GL_POINTS, 0, m_vertexCount);
         glBindVertexArray(0);
      }
	}

protected:
   GLuint m_vao;                       // Vertex Array Object
   GLuint m_vbo;                       // VBO
   float  m_pointSize;                 // Point size
   unsigned int m_vertexCount;         // Current vertex count
   unsigned int m_capacity;            // Maximum number of points
};

#endif