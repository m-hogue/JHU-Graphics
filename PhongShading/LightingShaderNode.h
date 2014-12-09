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

#ifndef __LIGHTINGSHADERNODE_H
#define __LIGHTINGSHADERNODE_H

#include <vector>
#include "scene/scene.h"

/**
 * Offset line shader node.
 */
class LightingShaderNode: public ShaderNode
{
public:
   /**
    * Gets uniform and attribute locations.
    */
   bool GetLocations() 
   {
      checkError("Top of GetLocations");

      m_positionLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "vertexPosition");
      if (m_positionLoc < 0)
      {
         printf("LightingShaderNode: Error getting vertex position location\n");
         return false;
      }
      m_vertexNormalLoc = glGetAttribLocation(m_shaderProgram.GetProgram(), "vertexNormal");
      if (m_vertexNormalLoc < 0)
      {
         printf("LightingShaderNode: Error getting color location\n");
         return false;
      }

      m_globalAmbientLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "globalLightAmbient");
      if (m_globalAmbientLoc < 0)
      {
         printf("LightingShaderNode: Error getting global ambient location\n");
         return false;
      }

      // Set the number of lights to 3
      numLights = 3;
      m_numLightsLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "numLights");
      if (m_numLightsLoc < 0)
      {
         printf("LightingShaderNode: Error getting numLightsLoc location\n");
         return false;
      }

      // Get light uniforms
      char name[128];
      for (int i = 0; i < numLights; i++)
      {
         sprintf(name, "lights[%d].enabled", i);
         m_lights[i].enabled = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].spotlight", i);
         m_lights[i].spotlight = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].position", i);
         m_lights[i].position = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].ambient", i);
         m_lights[i].ambient = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].diffuse", i);
         m_lights[i].diffuse = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].specular", i);
         m_lights[i].specular = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].constantAttenuation", i);
         m_lights[i].constantAttenuation = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].linearAttenuation", i);
         m_lights[i].linearAttenuation = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].quadraticAttenuation", i);
         m_lights[i].quadraticAttenuation = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].spotCosCutoff", i);
         m_lights[i].spotCosCutoff = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].spotExponent", i);
         m_lights[i].spotExponent = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
         sprintf(name, "lights[%d].spotDirection", i);
         m_lights[i].spotDirection = glGetUniformLocation(m_shaderProgram.GetProgram(), name);
      }
      
      // Populate matrix uniform locations in scene state
      m_pvmLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "pvm");
      m_modelMatrixLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "modelMatrix");
      m_normalMatrixLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "normalMatrix");
  
      // Populate material uniform locations in scene state 
      m_materialAmbientLoc   = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialAmbient");
      m_materialDiffuseLoc   = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialDiffuse");
      m_materialSpecularLoc  = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialSpecular");
      m_materialEmissionLoc  = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialEmission");
      m_materialShininessLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "materialShininess");

      // Populate camera position uniform location in scene state
      m_cameraPositionLoc = glGetUniformLocation(m_shaderProgram.GetProgram(), "cameraPosition");
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
      sceneState.m_numLightsLoc = m_numLightsLoc;
      sceneState.m_positionLoc = m_positionLoc;
      sceneState.m_normalLoc = m_vertexNormalLoc;
      sceneState.m_cameraPositionLoc = m_cameraPositionLoc;
      sceneState.m_pvmLoc = m_pvmLoc;
      sceneState.m_modelMatrixLoc = m_modelMatrixLoc;
      sceneState.m_normalMatrixLoc = m_normalMatrixLoc;
      sceneState.m_materialAmbientLoc = m_materialAmbientLoc;
      sceneState.m_materialDiffuseLoc = m_materialDiffuseLoc;
      sceneState.m_materialSpecularLoc = m_materialSpecularLoc;
      sceneState.m_materialEmissionLoc = m_materialEmissionLoc;
      sceneState.m_materialShininessLoc = m_materialShininessLoc;

      // Set the light locations
      for (unsigned int i = 0; i < numLights; i++)
         sceneState.lights[i] = m_lights[i];

      // Draw all children
		SceneNode::Draw(sceneState);
	}

   /**
    * Set the lighting
    */
   void SetGlobalAmbient(const Color4& globalAmbient) 
   {
      m_shaderProgram.Use();
      glUniform4fv(m_globalAmbientLoc, 1, &globalAmbient.r);
   }


   /**
    * Set light (used for Lighting and Viewing HW)
    */
   void SetLight(const unsigned int n, const HPoint3& position, const Color4& ambient,
                 const Color4& diffuse, const Color4& specular)
   {
      m_shaderProgram.Use();
      glUniform1i(m_lights[n].enabled, 1);
      glUniform4fv(m_lights[n].position, 1, &position.x);
      glUniform4fv(m_lights[n].ambient, 1, &ambient.r);
      glUniform4fv(m_lights[n].diffuse, 1, &diffuse.r);
      glUniform4fv(m_lights[n].specular, 1, &specular.r);
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
    * Get the location of the vertex position attribute.
    * @return  Returns the vertex position attribute location.
    */
   int GetNormalLoc() const
   {
      return m_vertexNormalLoc;
   }

protected:
   // Uniform and attribute locations
   GLint m_positionLoc;
   GLint m_vertexNormalLoc;
   GLint m_pvmLoc;
   GLint m_modelMatrixLoc;
   GLint m_normalMatrixLoc;
   GLint m_materialAmbientLoc;
   GLint m_materialDiffuseLoc;
   GLint m_materialSpecularLoc;
   GLint m_materialEmissionLoc;
   GLint m_materialShininessLoc;
   GLint m_cameraPositionLoc;
   GLint m_globalAmbientLoc;

   int numLights;
   GLint m_numLightsLoc;
   LightUniforms m_lights[3];
};

#endif