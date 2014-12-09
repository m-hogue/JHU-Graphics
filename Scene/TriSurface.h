//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	 David W. Nesbitt
//	File:     TransformNode.h
//	Purpose:  Scene graph geometry node indicating a triangle based,
//           curved surface.
//
//============================================================================

#ifndef __TRICURVEDSURFACE_H
#define __TRICURVEDSURFACE_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/**
 * Triangle mesh surface for use with curved surfaces. Uses 
 * vertex normals.  
 */
class TriSurface : public GeometryNode
{
public:
	/**
	 * Constructor. 
	 */
	TriSurface() 
   {
      m_vao = 0;
		m_vertexBuffer  = 0;
		m_faceBuffer    = 0;
		m_texCoordBuffer = 0;
   }
	
	/**
	 * Destructor.
	 */
	~TriSurface() 
   {
      // Delete vertex buffer objects
      glDeleteBuffers(1, &m_vertexBuffer);
      glDeleteBuffers(1, &m_faceBuffer);
	  glDeleteBuffers(1, &m_texCoordBuffer);
      glDeleteVertexArrays(1, &m_vao);
   }
	
   /**
    * Draw this geometry node.
    */
	void Draw(SceneState& sceneState)
   {
      glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_faceListCount, GL_UNSIGNED_SHORT, (void*)0);
      glBindVertexArray(0);
	}
	
	/**
	 * Construct triangle surface by passing in vertex list and face list
    * @param  vertexList  List of vertices (position and normal)
    * @param  faceList    Index list for triangles
	 */
	void Construct(std::vector<VertexAndNormal>& vertexList, std::vector<unsigned short> faceList, std::vector<Vector2> textureList)
	{
		m_vertexList = vertexList;
		m_faceList   = faceList;
		m_textureList = textureList;
	}

   /**
	 * Adds the vertices of the triangle to the vertex list. Accounts for
	 * shared vertices by checking if the vertex is already in the list.
	 * Manages the face list as well - stores indexes into the vertex list
	 * for this triangle. This method allows triangles to be added one at 
    * a time to a curved surface. Call End() when done to get vertex normal
    * assigned. Vertices should be oriented counter-clockwise.
    * @param  v0  First vertex in the triangle
    * @param  v1  Second vertex in the triangle
    * @param  v2  Last vertex in the triangle
	 */
	void Add(const Point3& v0, const Point3& v1, const Point3& v2)
	{
		m_faceList.push_back(addVertex(v0));
		m_faceList.push_back(addVertex(v1));
		m_faceList.push_back(addVertex(v2));
	}

   /**
	 * Marks the end of a triangle mesh. Calculates the vertex normals.
	 */
	void End(const int positionLoc, const int normalLoc, const int textureCoordLoc)
	{
		// Iterate through the face list and calculate the normals for each 
		// face and add the normal to each vertex in the face list. This 
		// assumes the vertex normals are initilaized to 0 (in constructor
		// of VertexAndNormal)
		unsigned int v0, v1, v2;
		Vector3 e1, e2, faceNormal;
		std::vector<unsigned short>::iterator faceVertex = m_faceList.begin();
		while (faceVertex != m_faceList.end())
		{
			// Get the vertices of the face (assumes ccw order)
			v0 = *faceVertex++;
			v1 = *faceVertex++;
			v2 = *faceVertex++;
			
			// Calculate surface normal. Normalize it since cross products
			// do not ensure unit length normals. We need to make sure normals
			// are same length so averaging works properly
			e1.Set(m_vertexList[v0].m_vertex, m_vertexList[v1].m_vertex);
			e2.Set(m_vertexList[v0].m_vertex, m_vertexList[v2].m_vertex);
			faceNormal = (e1.Cross(e2)).Normalize();
			
			// Add the face normal to the vertex normals of the triangle
			m_vertexList[v0].m_normal += faceNormal;
			m_vertexList[v1].m_normal += faceNormal;
			m_vertexList[v2].m_normal += faceNormal;
		}
		
		// Normalize the vertex normals - this essentially averages the adjoining face normals.
		std::vector<VertexAndNormal>::iterator v = m_vertexList.begin();
		for ( ; v != m_vertexList.end(); v++)
			v->m_normal.Normalize();
		
		// Create the vertex and face buffers
	   CreateVertexBuffers(positionLoc, normalLoc, textureCoordLoc);
	}
	
protected:
   // Vertex buffer support
   unsigned int m_faceListCount;
   GLuint m_vao;
	GLuint m_vertexBuffer;
	GLuint m_faceBuffer;
	GLuint		m_texCoordBuffer;
	// (s,t) texture list
	std::vector<Vector2> m_textureList;

   // Vertex and normal list
	std::vector<VertexAndNormal> m_vertexList;
	
	// Use unsigned short for face list indexes (OpenGL ES compatible)
	std::vector<unsigned short>  m_faceList;

   /**
    * Form triangle face indexes for a surface constructed using a double loop - one can be considered
    * rows of the surface and the other can be considered columns of the surface. Assumes the vertex
    * list is populated n "column" order. This helps conics and surface of revolution where angle is
    * incremented - can reduce number of trig calls.
    * @param  nrows  Number of rows
    * @param  ncols  Number of columns
    */
   void ConstructRowColFaceList(const unsigned int nrows, const unsigned int ncols)
   {
		for (unsigned int row = 0; row < nrows-1; row++)
		{
			for (unsigned int col = 0; col < ncols-1; col++)
			{
				// Divide each square into 2 triangles - make sure they are ccw.
				// GL_TRIANGLES draws independent triangles for each set of 3 vertices
				m_faceList.push_back((unsigned short)(col*nrows + row+1));
				m_faceList.push_back((unsigned short)(col*nrows + row));
				m_faceList.push_back((unsigned short)((col+1)*nrows + row));
				
				m_faceList.push_back((unsigned short)(col*nrows + row+1));
				m_faceList.push_back((unsigned short)((col+1)*nrows + row));
				m_faceList.push_back((unsigned short)((col+1)*nrows + row+1));
			}
		}
   }

   // Adds a vertex to the surface vertex list.  Returns the index into the
	// vertex list.  If the vertex is already in the list it does not
	// replicate it.
	unsigned int addVertex(const Point3& vIn)
	{
		// Check if vertex is in the list
		unsigned int index = 0;
		std::vector<VertexAndNormal>::iterator v = m_vertexList.begin();
		for ( ; v != m_vertexList.end(); v++, index++)
			if (vIn == v->m_vertex)
				return index;
		
		// Not in the list, add it. Make sure the vertex normal is initialized 
		// to (0,0,0)
		VertexAndNormal vertex(vIn);
		m_vertexList.push_back(vertex);
		m_textureList.push_back(Vector2(vIn.x, vIn.y));
		return (unsigned int)m_vertexList.size() - 1;
	}

   /**
    * Creates vertex buffers for this object.
    */
	void CreateVertexBuffers(const int positionLoc, const int normalLoc, const int texCoordLoc)
	{
      // Generate vertex buffers for the vertex list, face list, and texture coordinate list
		glGenBuffers(1, &m_vertexBuffer);
		glGenBuffers(1, &m_faceBuffer);
		 
		// Bind the texture list to the texture buffer object if there are textures
		if (m_textureList.size()>0){
			glGenBuffers(1, &m_texCoordBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, m_textureList.size() * sizeof(Vector2),
				(void*)&m_textureList[0], GL_STATIC_DRAW);
		}

      // Bind the vertex list to the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertexList.size() * sizeof(VertexAndNormal), 
                     (void*)&m_vertexList[0], GL_STATIC_DRAW);

      // Bind the face list to the vertex buffer object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faceList.size() * sizeof(unsigned short),
                     (void*)&m_faceList[0], GL_STATIC_DRAW);

      // Copy the face list count for use in Draw
      m_faceListCount = m_faceList.size();

      // We could clear any local memory as it is now in the VBO. However there may be
      // cases where we want to keep it (e.g. collision detection, picking) so I am not
      // going to do that here.

      // Allocate a VAO, enable it and set the vertex attribute arrays and pointers
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

      // Bind the vertex buffer, set the vertex position attribute and the vertex normal attribute
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
      glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (void*)0);
      glVertexAttribPointer(normalLoc,   3, GL_FLOAT, GL_FALSE, sizeof(VertexAndNormal), (void*)(sizeof(Point3)));
      glEnableVertexAttribArray(positionLoc);
      glEnableVertexAttribArray(normalLoc);

	  if (m_texCoordBuffer || m_textureList.size() > 0){
		  glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
		  if (m_texCoordBuffer)
			  glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
		  else if (m_textureList.size()>0)
			  glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)&m_textureList[0].x);
		  glEnableVertexAttribArray(texCoordLoc);
	  }

      // Bind the face list buffer and draw. Note the use of 0 offset in glDrawElements
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceBuffer);
	  
      // Make sure changes to this VAO are local
      glBindVertexArray(0);
	}
};


#endif