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
    * @param  capacity     Maximum number of line segments
    * @param  width        Line width
    * @param  positionLoc  Location of vertex position attribute
    * @param  colorLoc     Location of vertex color attribute
    */
	LineNode(const int capacity, const float width, const int positionLoc, const int colorLoc)
	{
      m_width      = width;
      m_colorCount = 0;
      m_capacity   = capacity;

      // Create vertex buffer objects for vertex positions and colors
      glGenBuffers(1, &m_vbo);
      glGenBuffers(1, &m_colorVBO);
    
      // Load dummy data into the vertex position VBO
      std::vector<LineSegment2> pts(capacity);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, capacity * 2 * sizeof(Point2),  (GLvoid*)&pts[0], GL_DYNAMIC_DRAW);

      // Load dummy data into the vertex color VBO to set capacity
      std::vector<Color3> colors(capacity*2);
      glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
      glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(Color3), (GLvoid*)&colors[0], GL_DYNAMIC_DRAW);
      
      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

      // Enable vertex attribute array and pointer so they are bound to the VAO
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(positionLoc);

      // Enable vertex attribute array and pointer so they are bound to the VAO
      glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
      glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(colorLoc);

      // Make sure changes to this VAO are local
      glBindVertexArray(0);
	}
	
   /**
    * Destructor
    */
	virtual ~LineNode() { }

   void SetWidth(const float w) 
   {
      m_width = w;
   }

   /**
    * Clear the line segments and colors.
    */
   void Clear()
   {
      m_lineSegments.clear();
      m_colorCount = 0;
   }

   /**
    * Add a line segment.
    * @param lineSegment  Line segment (endpoints).
    * @param c0  Color at vertex A on the segment.
    * @param c1  Color at vertex B on the segment.
    */
   void AddLineSegment(const LineSegment2& lineSegment, const Color3& c0, const Color3& c1)
   {
      if (m_lineSegments.size() >= m_capacity - 1)
      {
         printf("Exceeding capacity of lines in AddLineSegment\n");
         return;
      }

      //  Save the current number of prior line segments
      unsigned int n = m_lineSegments.size();

      // Add the line to the list
      m_lineSegments.push_back(lineSegment);

      // Update the vertex buffer object. Add just the new line segment (2 vertices)!
      // Set the offset based on number of previous lines (remember each line has 2 vertices)
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, n * 2 * sizeof(Point2), 2 * sizeof(Point2), (GLvoid*)&lineSegment);

      // Update the color buffer
      std::vector<Color3> colors;      // Vertex colors
      colors.push_back(c0);            // Color at the start of the line
      colors.push_back(c1);            // Color at the end of the line
      glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
      glBufferSubData(GL_ARRAY_BUFFER, m_colorCount * sizeof(Color3), 2 * sizeof(Color3), (GLvoid*)&colors[0]);
      m_colorCount += 2;
   }

   /**
    * Replace the line segment at position 0 with this line segment. Used for drawing
    * a single line (the current line).
    * @param lineSegment  Line segment
    * @param c0  Color at start of the segment
    * @param c1  Color at the end of the segment
    */
   void Replace(const LineSegment2& lineSegment, const Color3& c0, const Color3& c1)
   {
      m_lineSegments.clear();
      m_lineSegments.push_back(lineSegment);

      // Update the vertex buffer object. Add just the new vertices
      // Set the offset based on number of previous vertices
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(Point2), (GLvoid*)&lineSegment);

      // Update the color buffer
      std::vector<Color3> colors;      // Vertex colors
      colors.push_back(c0);            // Color at the start of the line
      colors.push_back(c1);            // Color at the end of the line
      glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(Color3), (GLvoid*)&colors[0]);
      m_colorCount = 2;
   }

   /**
    * Get a reference to the list of line segments.
    * @return  Returns a reference to the line segment list.
    */
   std::vector<LineSegment2>& GetLineSegments()
   {
      return m_lineSegments;
   }

	/**
	 * Draw the lines
    * @param  sceneState  Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      if (m_lineSegments.size() > 0)
      {
         // Line width
         checkError("LineNode - before width");
         glLineWidth(m_width);
         checkError("LineNode - after width");

         // Draw - the count in glDrawArrays is the number of vertices in the list, 
         // not the number of line segments.
         glBindVertexArray(m_vao);
         glDrawArrays(GL_LINES, 0, m_lineSegments.size() * 2);
         glBindVertexArray(0);
      }
	}

protected:
   GLuint m_vao;                             // Vertex Array Object
   GLuint m_vbo;                             // VBO for positions
   GLuint m_colorVBO;                        // VBO for vertex colors
   float  m_width;                           // Line width
	std::vector<LineSegment2> m_lineSegments; // List of line segments
   unsigned int m_colorCount;                // Should equal 2 * number of lines
   unsigned int m_capacity;                  // Maximum number of lines
};

#endif