//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ShaderNode.h
//	Purpose:	Scene graph shader node.
//
//============================================================================

#ifndef __SHADERNODE_H
#define __SHADERNODE_H

#include <list>
#include "geometry/geometry.h"

/**
 * Shader node. Enables a shader program. The program is loaded with
 * different constructor methods. Derived shader node classes should 
 * provide specialization to control uniforms and attributes.
 */
class ShaderNode: public SceneNode
{
public:
   /**
    * Constructor.
    */
	ShaderNode()
   { 
      m_nodeType = SCENE_SHADER; 
   }

   /**
    * Destructor.
    */
	virtual ~ShaderNode() { }

   /**
    * Create a shader program given a filename for the vertex shader and a filename
    * for the fragment shader.
    * @param  vertexShaderFilename    Vertex shader file name
    * @param  fragmentShaderFilename  Fragment shader file name
    * @return  Returns true if successful, false if compile or link errors occur.
    */
   bool Create(const char* vertexShaderFilename, const char* fragmentShaderFilename)
   {
      // Create and compile the vertex shader
      if (!m_vertexShader.Create(vertexShaderFilename))
      {
         printf("Vertex Shader compile failed\n");
         return false;
      }

      // Create and compile the fragment shader
      if (!m_fragmentShader.Create(fragmentShaderFilename))
      {
         printf("Fragment Shader compile failed\n");
         return false;
      }

      m_shaderProgram.Create();
      if (!m_shaderProgram.AttachShaders(m_vertexShader.Get(), m_fragmentShader.Get()))
      {
         printf("Shader program link failed\n");
         return false;
      }
      return true;
   }

   /**
    * Create a shader program given source char array for the vertex shader and source
    * for the fragment shader.
    * @param  vertexShaderSource    Vertex shader source (char array)
    * @param  fragmentShaderSource  Fragment shader source (char array)
    * @return  Returns true if successful, false if compile or link errors occur.
    */
   bool CreateFromSource(const char* vertexShaderSource, const char* fragmentShaderSource)
   {
      // Create and compile the vertex shader
      if (!m_vertexShader.CreateFromSource(vertexShaderSource))
      {
         printf("Vertex Shader compile failed\n");
         return false;
      }

      // Create and compile the fragment shader
      if (!m_fragmentShader.CreateFromSource(fragmentShaderSource))
      {
         printf("Fragment Shader compile failed\n");
         return false;
      }

      m_shaderProgram.Create();
      if (!m_shaderProgram.AttachShaders(m_vertexShader.Get(), m_fragmentShader.Get()))
      {
         printf("Shader program link failed\n");
         return false;
      }
      return true;
   }

   // Derived classes must add this to set all internal uniforms and attribute locations
   virtual bool GetLocations() = 0;

protected:
   GLSLVertexShader   m_vertexShader;
   GLSLFragmentShader m_fragmentShader;
   GLSLShaderProgram  m_shaderProgram;
};

#endif