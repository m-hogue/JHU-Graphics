//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GLSL fragment shader class
//	Purpose: Support for GLSL fragment shaders. Supports loading, compiling, 
//           linking, attaching, etc.
//============================================================================

#ifndef __GLSLFRAGMENTSHADER_H__
#define __GLSLFRAGMENTSHADER_H__

#include <stdio.h>
#include <stdarg.h>

/**
 * GLSL vertex shader
 */
class GLSLFragmentShader : public GLSLShader
{
public:
   GLSLFragmentShader()  { }
   ~GLSLFragmentShader() { }

   /**
    * Create fragment shader from source code chararacter array.
    * @param  fragSource  Source code for the fragment shader.
    * @return  Returns true if successful, false if not.
    */
   bool CreateFromSource(const char* fragSource)
   {
      bool success = true;
      m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(m_fragmentShader, 1, &fragSource, NULL);
      glCompileShader(m_fragmentShader);
      if (!CheckCompileStatus(m_fragmentShader))
      {
         printf("Fragment shader compile failed\n");
         printf("Source: %s\n", fragSource);
         logCompileError(m_fragmentShader);
         success = false;
      }
      return success;
   }

   /**
    * Create fragment shader from source code file.
    * @param  fname File name for the source code for the fragment shader.
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
    * Get the fragment shader handle.
    * @return Returns a handle to the fragment shader.
    */
   GLuint Get() const
   {
      return m_fragmentShader;
   }

protected:
   GLuint m_fragmentShader;
};

#endif
