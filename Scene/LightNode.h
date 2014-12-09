//============================================================================
//
//	Author:	David W. Nesbitt
//	File:    LightNode.h
//	Purpose: Scene graph light node.
//
//============================================================================

#ifndef __LIGHTNODE_H_
#define __LIGHTNODE_H_

#include <list>

/**
 * Presentation node. Applies material and texture
 */
class LightNode : public SceneNode
{
public:
	/**
	 * Constructor
	 */
	LightNode(const unsigned int index) 
	{
		m_nodeType = SCENE_LIGHT;
		m_index = index;

      // Set default attenuation to 1 and spotlight to false
      m_enabled = false;
      m_atten0 = 1.0f;
      m_atten1 = 0.0f;
      m_atten2 = 0.0f;
      m_isSpotlight = false;
		
		// Note: color constructors default rgb to 0 and alpha to 1
	}
	
	/**
	 * Destructor
	 */
	~LightNode() { }
	
	/** 
	 * Enable this light source
	 */
	void Enable()
	{
		m_enabled = true;
	}
	
	/** 
	 * Disable this light source
	 */
	void Disable()
	{
		m_enabled = false;
	}
		
	/**
	 * Set ambient light illumination (color).
	 */
	void SetAmbient(const Color4& c)
	{
		m_ambient = c;
	}
	
	/**
	 * Set diffuse light illumination (color).
	 */
	void SetDiffuse(const Color4& c)
	{
		m_diffuse = c;
	}
	
	/**
	 * Set specular light illumination (color).
	 */
	void SetSpecular(const Color4& c)
	{
		m_specular = c;
	}
	
	/**
	 * Set the light position. Uses a homogeneous coordinate. If w = 0 the light is 
	 * directional. Position is set within the Draw method.
	 */
	void SetPosition(const HPoint3& position)
	{
		m_position = position;

      // If directional light - make sure the position represents a unit vector
      if (m_position.w == 0.0f)
      {
         Vector3 L(m_position.x, m_position.y, m_position.z);
         L.Normalize();
         m_position.x = L.x;
         m_position.y = L.y;
         m_position.z = L.z;
      }
	}

   /**
    * Set spotlight parameters
    */
   void SetSpotlight(const Vector3& spotDir, const float spotExponent, const float spotCutoff)
   {
      m_spotDirection = spotDir;
      m_spotExponent  = spotExponent;
      m_cosSpotCutoff = cosf(degreesToRadians(spotCutoff));
      m_isSpotlight   = true;
   }

   /**
    * Set the spotlight direction
    */
   void SetSpotlightDirection(const Vector3& spotDir)
   {
      m_spotDirection = spotDir;
   }
	
   /**
    * Disable spotlight
    */
   void TurnOffSpotlight() 
   {
      m_isSpotlight = false;
   }

   /**
    * Set attenuation
    */
   void SetAttenuation(const float constant, const float linear, const float quadratic)
   {
      m_atten0 = constant;
      m_atten1 = linear;
      m_atten2 = quadratic;
   }

	/**
	 * Draw. Sets the light properties if enabled. Note that only position
    * is set within the Draw method - since it needs to be transformed by
    * the current matrix. 
	 */
	void Draw(SceneState& sceneState)
	{
      glUniform1i(sceneState.lights[m_index].enabled, (int)m_enabled);
		if (m_enabled)
		{
         glUniform1i(sceneState.lights[m_index].spotlight, (int)m_isSpotlight);
         glUniform4fv(sceneState.lights[m_index].position, 1, &m_position.x);
         glUniform4fv(sceneState.lights[m_index].ambient, 1, &m_ambient.r);
         glUniform4fv(sceneState.lights[m_index].diffuse, 1, &m_diffuse.r);
         glUniform4fv(sceneState.lights[m_index].specular, 1, &m_specular.r);
         glUniform1f(sceneState.lights[m_index].constantAttenuation, m_atten0);
         glUniform1f(sceneState.lights[m_index].linearAttenuation, m_atten1);
         glUniform1f(sceneState.lights[m_index].quadraticAttenuation, m_atten2);
         if (m_isSpotlight)
         {
            // Note we use cos of the spotlight cutoff angle so we don't have to compute cos 
            // in the shader
            glUniform1f(sceneState.lights[m_index].spotCosCutoff, m_cosSpotCutoff);
            glUniform3fv(sceneState.lights[m_index].spotDirection, 1, &m_spotDirection.x);
            glUniform1f(sceneState.lights[m_index].spotExponent, m_spotExponent);
         }

         if (m_index > (unsigned int)sceneState.m_maxEnabledLight)
         {
            glUniform1i(sceneState.m_numLightsLoc, m_index+1);
            sceneState.m_maxEnabledLight = m_index;
         }
      }

		// Draw children of this node
		SceneNode::Draw(sceneState);

      // To be proper we should disable this light so it does not impact any nodes that 
      // are not descended from this node
      glUniform1i(sceneState.lights[m_index].enabled, 0);
	}
	
protected:
	bool         m_enabled;
   bool         m_isSpotlight;
	unsigned int m_index;
	Color4       m_ambient;
	Color4       m_diffuse;
	Color4       m_specular;
   Vector3      m_spotDirection;
   float        m_cosSpotCutoff;
   float        m_spotExponent;
   float        m_atten0;
   float        m_atten1; 
   float        m_atten2;
	
	// Light position as a homogeneous coordinate. If w = 0 the light is directional
	HPoint3      m_position;
};

#endif