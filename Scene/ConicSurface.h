//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ConicSurface.h
//	Purpose: Scene graph geometry node representing a conic surface.
//
//============================================================================

#ifndef __CONICSURFACE_H
#define __CONICSURFACE_H

/**
 * Conic surface geometry node.
 */
class ConicSurface : public TriSurface
{
public:
	/**
    * Creates a conic surface (cone, cylinder) with specified bottom radius,
    * top radius, number of sides (how many times the top/bottom circles
    * are divided), and number of stacks (subdivisions in z).  z values range 
    * from -0.5 to 0.5 (unit height). If either the bottom or top radius is 0,
    * a cone outer surface is created. If bottomRadius and topRadius are equal, 
    * a cylinder surface is created. End caps are not included!
    *
    * @param   bottomRadius   Radius of bottom
    * @param   topRadius      Radius of top
    * @param   nSides         Number of sides (divisions of the top/bottom)
    * @param   nStacks        Number of height divisions
    */
	ConicSurface(const float bottomRadius,  const float topRadius, 
                const unsigned int nSides, const unsigned int nStacks,
                const int positionLoc, const int normalLoc, const int textureLoc)
	{
      // Fail if top and bottom radius are both 0
      if (bottomRadius <= 0.0f && topRadius <= 0.0f)
         return;
      
      // There are nStacks+1 rows in the vertex list
      m_nRows = nStacks + 1;

      // Set a rotation matrix for the normals
      Matrix4x4 m;
      m.Rotate(360.0f / (float)nSides, 0.0f, 0.0f, 1.0f);

      // Create a normal at theta = 0 perpendicular to vector along side. Note that if
      // we use a 2D vector in the x,z plane to represent the side vector then we just
      // swap vertices and negate to find a perpendicular
      Vector3 n(1.0, 0.0, (bottomRadius - topRadius));
      n.Normalize();

      // Create a vertex list. Note that normals can be computed based
      // on the difference in bottom and top radius. Note that we 
      // change radius linearly from the bottom radius to top radius
      unsigned int i, j;
      float s = 0.0f;
      float t;
      float z, r;
      float dz = 1.0f / nStacks;
      float dr = (topRadius - bottomRadius) / nStacks;
      float theta = 0.0f;
      float dtheta = (2.0f * (float)M_PI) / (float)nSides;
      float cosTheta, sinTheta;
      float ds = 1.0f / (float)nSides;
      float dt = 1.0f / (float)nStacks;
      VertexAndNormal vtx;
      for (i = 0; i < nSides; i++, theta += dtheta, s += ds)
      {
         // Compute sin,cos to use in the loop below
         cosTheta = cosf(theta);
         sinTheta = sinf(theta);

         // Set the normal for this side
         vtx.m_normal = n;

         // Form vertices along the side for this angle increment
         for (j = 0, z = -0.5f, t = 0.0f, r = bottomRadius; j <= nStacks; j++, z += dz, r += dr, t += dt)
         {
            vtx.m_vertex.Set(r * cosTheta, r * sinTheta, z);
            m_vertexList.push_back(vtx);
			m_textureList.push_back(Vector2(theta / M_PI, z));
         }

         // Rotate the normal
         n = m * n;
      }

      // Copy the first column of vertices
      for (i = 0; i < m_nRows; i++)
         m_vertexList.push_back(m_vertexList[i]);

      // Form triangle face indexes. Note: there are n+1 rows 
		// and n+1 columns.
		m_nCols = nSides + 1;
		for (unsigned int row = 0; row < m_nRows - 1; row++)
		{
			for (unsigned int col = 0; col < m_nCols - 1; col++)
			{
				// Divide each square into 2 triangles - make sure they are ccw.
				// GL_TRIANGLES draws independent triangles for each set of 3 vertices
				m_faceList.push_back(getIndex(row+1, col));
				m_faceList.push_back(getIndex(row, col));
				m_faceList.push_back(getIndex(row, col+1));
				
				m_faceList.push_back(getIndex(row+1, col));
				m_faceList.push_back(getIndex(row, col+1));
				m_faceList.push_back(getIndex(row+1, col+1));
			}
		}

      // Connect the last column with the first
      for (unsigned int row = 0; row < m_nRows - 1; row++)
      {
			m_faceList.push_back(getIndex(row+1, m_nCols - 1));
			m_faceList.push_back(getIndex(row,   m_nCols - 1));
			m_faceList.push_back(getIndex(row,   0));
				
			m_faceList.push_back(getIndex(row+1, m_nCols - 1));
			m_faceList.push_back(getIndex(row,   0));
			m_faceList.push_back(getIndex(row+1, 0));
      }

      // Create vertex buffers
      CreateVertexBuffers(positionLoc, normalLoc, textureLoc);
	}
	
private:
   unsigned int m_nRows;
   unsigned int m_nCols;

	// Make default constructor private to force use of the constructor
	// with number of subdivisions.
	ConicSurface() { };

   // Convenience method to get the index into the vertex list given the
	// "row" and "column" of the subdivision/grid
	unsigned int getIndex(unsigned int row, unsigned int col) const
	{
		return (col*m_nRows) + row;
	}
};

#endif