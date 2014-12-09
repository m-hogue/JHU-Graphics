//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PointShaderNode.h
//	Purpose:	Derived class to handle a point shader and its uniforms and 
//          attribute locations.
//
//============================================================================

#ifndef __POINTSHADERNODE_H
#define __POINTSHADERNODE_H

#include <vector>
#include "scene/scene.h"

/**
 *Point shader node.
 */
class PointShaderNode: public ShaderNode
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
      m_positionLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "position");
      if (m_positionLoc < 0)
      {
         printf("Error getting vertex position location\n");
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
      sceneState.m_colorLoc = m_colorLoc;
      sceneState.m_positionLoc = m_positionLoc;

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

protected:
   // Uniform and attribute locations used by this shader
   GLint m_orthoMatrixLoc;
   GLint m_colorLoc;
   GLint m_positionLoc;
};

#endif