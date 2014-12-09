//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GLSL vertex shader class
//	Purpose: Support for GLSL vertex shaders. Supports loading, compiling, 
//           linking, attaching, etc.
//
//============================================================================

#ifndef __GLSLVERTEXSHADER_H__
#define __GLSLVERTEXSHADER_H__

#include <stdio.h>
#include <stdarg.h>

/**
 * GLSL vertex shader
 */
class GLSLVertexShader : public GLSLShader
{
public:
   GLSLVertexShader()  { }
   ~GLSLVertexShader() { }

   /**
    * Create vertex shader from source code chararacter array.
    * @param  vertSource  Source code for the vertex shader.
    * @return  Returns true if successful, false if not.
    */
   bool CreateFromSource(const char* vertSource)
   {
      bool success = true;
      m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(m_vertexShader, 1, &vertSource, NULL);
      glCompileShader(m_vertexShader);
      if (!CheckCompileStatus(m_vertexShader))
      {
         printf("Vertex shader compile failed\n"); 
         logCompileError(m_vertexShader);
         success = false;
      }
      return success;
   }

   /**
    * Create vertex shader from source code file.
    * @param  fname File name for the source code for the vertex shader.
    * @return  Returns true if successful, false if not.
    */
   bool Create(const char* fname)
   {
      char* source = readShaderSource(fname);
      bool success = CreateFromSource(source);
      delete [] source;
      return success;
   }

   /**
    * Get the vertex shader handle.
    * @return Returns a handle to the vertex shader.
    */
   GLuint Get() const
   {
      return m_vertexShader;
   }

protected:
   GLuint m_vertexShader;
};

#endif
