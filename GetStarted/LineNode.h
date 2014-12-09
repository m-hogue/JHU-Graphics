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
#include "scene/scene.h"

/**
 * Unit sphere geometry node.
 */
class LineNode: public GeometryNode
{
public:
   /**
    * Constructor.
    */
	LineNode(std::vector<Point2> pts, const Color4& color, const int positionLoc, const int offsetLoc)
	{
      // Copy the color
      m_color = color;

      // Set default width 
      m_width = 2.0f;

      std::vector<Point2>  vertexList;  // Vertex list
      std::vector<Vector2> offsetList;  // Vertex offset list

      // Form first offset vector
      Vector2 v1(pts[0], pts[1]);
      v1.Normalize();
      Vector2 perp1 = v1.GetPerpendicular();
      add(pts[0], perp1, vertexList, offsetList);
      
      // Iterate through the list and form joins
      int n = pts.size() - 1;
      Vector2 v2, perp2;
      for (int i = 0; i < n; i++)
      {
         // Form the segment and normalize it
         v2.Set(pts[i], pts[i+1]);
         v2.Normalize();

         // Form a unit length vector tyhat is the average of the 2 segment perpendiculars
         // Scale the vector by projecting avg onto the normal and taking the inverse
         perp2 = v2.GetPerpendicular();
         Vector2 avg = (perp1 + perp2).Normalize();
         avg *= 1.0f / avg.Dot(perp2);
         add(pts[i], avg, vertexList, offsetList);

         // Copy the vectors for the next iteration
         perp1 = perp2;
      }

      // Add the last position and offset vector
      add(pts[n], perp2, vertexList, offsetList);
      m_vertexCount = vertexList.size();

      // Create buffer objects
      glGenBuffers(1, &m_vbo);
      glGenBuffers(1, &m_offsetVBO);

      // Add the points to the buffer
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Point2),  (GLvoid*)&vertexList[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, m_offsetVBO);
      glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vector2), (GLvoid*)&offsetList[0], GL_STATIC_DRAW);

      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glEnableVertexAttribArray(positionLoc);
      glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, m_offsetVBO);
      glEnableVertexAttribArray(offsetLoc);
      glVertexAttribPointer(offsetLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	
   /**
    * Destructor
    */
	virtual ~LineNode()
   {
      glDeleteBuffers(1, &m_vbo);
      glDeleteVertexArrays(1, &m_vao);
   }

   void SetWidth(const float w) 
   {
      m_width = w;
   }

	/**
	 * Draw the lines
    * @param  sceneState  Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      // Set the color
      glUniform4f(sceneState.m_colorLoc, m_color.r, m_color.g, m_color.b, m_color.a);

      // Set the line width. Set half the width since the offset vector is scaled to both sides
      glUniform1f(sceneState.m_widthLoc, m_width*0.5f);

      // Bind the VAO and draw the lines
      glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
      glBindVertexArray(0);
	}

protected:
   unsigned int m_vertexCount;         // Number of vertices
   GLuint m_vbo;                       // VBO for positions
   GLuint m_vao;                       // Vertex Array Object
   GLuint m_offsetVBO;                 // VBO for offset vectors
   float  m_width;                     // Line width
   Color4 m_color;                     // Color to draw the points


   // Convenience method to add a point and its offset vector. Repeats the point and adds
   // the negative of the offset vector
   void add(const Point2& p, const Vector2 v, std::vector<Point2>& vertexList,  std::vector<Vector2>& offsetList)
   {
      offsetList.push_back(v);
      vertexList.push_back(p);
      offsetList.push_back(v * -1.0f);
      vertexList.push_back(p);
   }
};

#endif