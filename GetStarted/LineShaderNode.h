//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    LineShaderNode.h
//	Purpose:	Derived class to handle an offset line shader and its uniforms and 
//          attribute locations.
//
//============================================================================

#ifndef __LINESHADERNODE_H
#define __LINESHADERNODE_H

#include <vector>
#include "scene/scene.h"

/**
 * Offset line shader node.
 */
class LineShaderNode: public ShaderNode
{
public:
   /**
    * Gets uniform and attribute locations.
    */
   bool GetLocations() 
   {
      m_orthoMatrixLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "ortho");
      if (m_orthoMatrixLoc < 0)
      {
         printf("Error getting ortho matrix location\n");
         return false;
      }
      m_colorLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "color");
      if (m_colorLoc < 0)
      {
         printf("Error getting color location\n");
         return false;
      }
      m_widthLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "width");
      if (m_widthLoc < 0)
      {
         printf("Error getting width location\n");
         return false;
      }
      m_positionLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "position");
      if (m_positionLoc < 0)
      {
         printf("Error getting vertex position location\n");
         return false;
      }
      m_offsetLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "offset");
      if (m_offsetLoc < 0)
      {
         printf("Error getting vertex offset location\n");
         return false;
      }
      return true;
   }

	/**
    * Draw method for this shader - enable the program and set up uniforms
    * and vertex attribute locations
    * @param  sceneState   Current scene state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      // Enable this program
      m_shaderProgram.Use();

      // Set scene state locations to ones needed for this program
      sceneState.m_orthoMatrixLoc = m_orthoMatrixLoc;
      sceneState.m_colorLoc    = m_colorLoc;
      sceneState.m_widthLoc    = m_widthLoc;
      sceneState.m_positionLoc = m_positionLoc;
      sceneState.m_offsetLoc   = m_offsetLoc;

      // Set the matrix
      glUniformMatrix4fv(m_orthoMatrixLoc, 1, GL_FALSE, sceneState.m_ortho);

      // Draw all children
		SceneNode::Draw(sceneState);
	}

   /**
    * Get the vertex position attribute location
    * @return  Returns the position attribute location.
    */
   int GetPositionLoc() const
   {
      return m_positionLoc;
   }

   /**
    * Get the vertex offset attribute location
    * @return  Returns the offset attribute location.
    */
   int GetOffsetLoc() const
   {
      return m_offsetLoc;
   }

protected:
   // Uniform and attribute locations
   GLint m_orthoMatrixLoc;
   GLint m_colorLoc;
   GLint m_widthLoc;
   GLint m_positionLoc;
   GLint m_offsetLoc;
};

#endif