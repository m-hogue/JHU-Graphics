//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Base GLSL shader class
//	Purpose: Support for loadng a shader from a file and compiling it
//
//============================================================================

#ifndef __GLSLSHADER_H__
#define __GLSLSHADER_H__

#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#define _lseek lseek
#define _open open
#define _close close
#endif

/**
 * Base shader class. Loads from file. Checks compile status.
 */
class GLSLShader
{
public:
   GLSLShader()  { }
   ~GLSLShader() { }

   bool CheckCompileStatus(GLuint shader)
   {
      int param = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
      return (param == GL_TRUE);
   }

protected:
   // Utility to read a shader source file
   char* readShaderSource(const char* filename) 
   {
      if (filename == 0)
      {
         printf("NULL filename for shader...exiting\n");
         exit(-1);
      }

      std::string fname = filename;
      char* content = NULL;
  
      // Get file size
      int f = _open(fname.c_str(), O_RDONLY);
      if (f < 0) 
      {
         // Try the parent directory
         fname.insert(0, "../");
         f = _open(fname.c_str(), O_RDONLY);
         if (f < 0)
         {
            printf("Could not open shader file %s. Also not in parent directory\n", filename);
            exit(-1);
         }
      }
      int count = _lseek(f, 0, SEEK_END);
      _close(f);

      FILE* fp = fopen(fname.c_str(), "rt");
      if (fp != NULL) 
      {
         if (count > 0) 
         {
            content = (char *)new char[count+1];
            count = (int)fread(content, sizeof(char), count, fp);
            content[count] = '\0';
         }
         fclose(fp);
      }
      else
         printf("Could not read file %s\n", filename);
      
      return content;
   } 

   /**
    * Logs a shader compile error
    */
   void logCompileError(GLuint shader)
   {
      GLint    len = 0; 
      GLsizei slen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &len); 
      if (len > 1)
      {
         GLchar* compilerLog = (GLchar*)new GLchar*[len];
         glGetShaderInfoLog(shader, len, &slen, compilerLog);
         printf("compiler log:\n%s", compilerLog);
         delete [] compilerLog;
      }
   }
};

// Include derived classes so application only add this one file
#include "ShaderSupport/GLSLFragmentShader.h"
#include "ShaderSupport/GLSLVertexShader.h"
#include "ShaderSupport/GLSLShaderProgram.h"

#endif
