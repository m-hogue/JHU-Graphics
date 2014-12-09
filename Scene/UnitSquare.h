//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    UnitSquare.h
//	Purpose: Scene graph geometry node representing a unit square.
//
//============================================================================

#ifndef __UNITSQUARE_H
#define __UNITSQUARE_H

class UnitSquareSurface : public TriSurface
{
public:
	/**
	 * Creates a unit length and width "flat surface".  The surface is composed of
	 * triangles such that the unit length/width surface is divided into n
	 * equal paritions in both x and y. Constructs a vertex list and face list
	 * for the surface.
    * @param  n   Number of subdivisions in x and y
	 */
	UnitSquareSurface(unsigned int n, const int positionLoc, const int normalLoc, const int textureLoc)
	{
		// Only allow 250 subdivision (so it creates less that 65K vertices)
		if (n > 250)
			n = 250;
		
      // Normal is 0,0,1. z = 0 so all vertices lie in x,y plane.
		// Having issues with roundoff when n = 40,50 - so compare with some tolerance
      VertexAndNormal vtx;
      vtx.m_normal.x = 0.0f;
      vtx.m_normal.y = 0.0f;
      vtx.m_normal.z = 1.0f;
      vtx.m_vertex.z = 0.0f;
		float spacing = 1.0f / n;
		for (vtx.m_vertex.x = -0.5; vtx.m_vertex.x <= 0.5f + EPSILON; vtx.m_vertex.x += spacing) {
			for (vtx.m_vertex.y = -0.5; vtx.m_vertex.y <= 0.5f + EPSILON; vtx.m_vertex.y += spacing) {
				// shift texture by 0.5 so that mapping space is [0,1]
				m_textureList.push_back(Vector2(vtx.m_vertex.x + 0.5f, vtx.m_vertex.y + 0.5f));
				m_vertexList.push_back(vtx);
			}
		}
		
      // Construct face list and create vertex buffer objects
      ConstructRowColFaceList(n+1, n+1);
      CreateVertexBuffers(positionLoc, normalLoc, textureLoc);
	}
	
private:
	unsigned int m_nCols;
	
	// Convenience method to get the index into the vertex list given the
	// "row" and "column" of the subdivision/grid
	unsigned int getIndex(unsigned int row, unsigned int col) const
	{
		return (row*m_nCols) + col;
	}
	
	// Make default constructor private to force use of the constructor
	// with number of subdivisions.
	UnitSquareSurface() { };
};

#endif