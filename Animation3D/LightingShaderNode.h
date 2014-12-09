//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    LightingShaderNode.h
//	Purpose: Derived class to handle the lighting shader program.
//
//============================================================================

#ifndef __LIGHTINGSHADERNODE_H
#define __LIGHTINGSHADERNODE_H

#include <vector>

/**
 * Simple lighting shader node.
 */
class LightingShaderNode: public ShaderNode
{
public:
   /**
    * Gets uniform and attribute locations.
    */
   bool GetLocations() 
   {
      m_positionLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "vertexPosition");
      if (m_positionLoc < 0)
      {
         printf("Error getting vertexPosition location\n");
         return false;
      }
      m_vertexNormalLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "vertexNormal");
      if (m_vertexNormalLoc < 0)
      {
         printf("Error getting vertexNormal location\n");
         return false;
      }
      m_materialColorLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialColor");
      if (m_materialColorLoc < 0)
      {
         printf("Error getting materialColor location\n");
         return false;
      }
      m_pvmLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "pvm");
      if (m_pvmLoc < 0)
      {
         printf("Error getting pvm location\n");
         return false;
      }
      m_modelViewMatrixLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "modelViewMatrix");
      if (m_modelViewMatrixLoc < 0)
      {
         printf("Error getting modelViewMatrix location\n");
         return false;
      }

      m_normalMatrixLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "normalMatrix");
      if (m_normalMatrixLoc < 0)
      {
         printf("Error getting normalMatrix location\n");
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
      sceneState.m_positionLoc        = m_positionLoc;
      sceneState.m_normalLoc          = m_vertexNormalLoc;
      sceneState.m_materialDiffuseLoc = m_materialColorLoc;
      sceneState.m_pvmLoc             = m_pvmLoc;
      sceneState.m_modelViewMatrixLoc = m_modelViewMatrixLoc;
      sceneState.m_normalMatrixLoc    = m_normalMatrixLoc;

      // Draw all children
      SceneNode::Draw(sceneState);
	}

   /**
    * Get the location of the vertex position attribute.
    * @return  Returns the vertex position attribute location.
    */
   int GetPositionLoc() const
   {
      return m_positionLoc;
   }

   /**
    * Get the location of the vertex normal attribute.
    * @return  Returns the vertex normal attribute location.
    */
   int GetNormalLoc() const
   {
      return m_vertexNormalLoc;
   }

protected:
   // Uniform and attribute locations:
   GLint m_positionLoc;	         // Vertex position attribute location
   GLint m_vertexNormalLoc;		// Vertex normal attribute location
   GLint m_materialColorLoc;	   // Material diffuse color location
   GLint m_pvmLoc;				   // Composite projection, view, model matrix location
   GLint m_modelViewMatrixLoc;	// ModelView composite matrix location
   GLint m_normalMatrixLoc;		// Normal transformation matrix location
};

#endif