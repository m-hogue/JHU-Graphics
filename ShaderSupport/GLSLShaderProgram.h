//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GLSL shader program class
//	Purpose: Support for GLSL shader programs
//============================================================================

#ifndef __GLSLSHADERPROGRAM_H__
#define __GLSLSHADERPROGRAM_H__

#include <stdio.h>
#include <stdarg.h>

/**
 * GLSL shader program
 */
class GLSLShaderProgram
{
public:
   GLSLShaderProgram()  
   { 
      m_shaderProgram = 0;
   }

   ~GLSLShaderProgram() { }

   /**
    * Create a shader program
    */
   void Create()
   {
      m_shaderProgram = glCreateProgram();
   }

   /**
    * Attach the specified shaders.
    */
   bool AttachShaders(GLuint vertexShader, GLuint fragmentShader)
   {
   	glAttachShader(m_shaderProgram, vertexShader);
	   glAttachShader(m_shaderProgram, fragmentShader);
	   glLinkProgram(m_shaderProgram);
      if (!CheckLinkStatus())
      {
         printf("Shader link failed\n");
         logLinkError();
         return false;
      }
      return true;
   }

   /**
    * Get the shader program handle
    * @return  Returns the handle to the shader program.
    */
   GLuint GetProgram() const
   {
      return m_shaderProgram;
   }

   /**
    * Use this shader program.
    */
   void Use() 
   {
      glUseProgram(m_shaderProgram);
   }

protected:
   GLuint m_shaderProgram;

   /**
    * Checks the link status.
    * @return  Returns true if hte linker was successful, false if an error occured.
    */
   bool CheckLinkStatus()
   {
      int param = 0;
      glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &param);
      return (param == GL_TRUE);
   }

   /**
    * Logs a shader program link error to the console window.
    */
   void logLinkError()
   {
      GLint    len = 0; 
      GLsizei slen = 0;
      glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH , &len); 
      if (len > 1)
      {
         GLchar* linkLog = (GLchar*)new GLchar*[len];
         glGetProgramInfoLog(m_shaderProgram, len, &slen, linkLog);
         printf("Program Link Log:\n%s", linkLog);
         delete [] linkLog;
      }
   }
};

#endif
