//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    SceneState.h
//	Purpose:	Class used to propogate state during traversal of the scene graph.
//
//============================================================================

#ifndef __SCENESTATE_H
#define __SCENESTATE_H

#include <list>

// Simple structure to hold light uniform locations
struct LightUniforms
{
   GLint enabled;
   GLint spotlight;
   GLint position;
   GLint ambient;
   GLint diffuse;
   GLint specular;
   GLint constantAttenuation;
   GLint linearAttenuation;
   GLint quadraticAttenuation;
   GLint spotCosCutoff;
   GLint spotExponent;
   GLint spotDirection;
};

class SceneState
{
public:
   // Vertex attribute locations
   GLint m_positionLoc;                // Vertex position attribute location
   GLint m_offsetLoc;                  // Vertex offset location
   GLint m_normalLoc;                  // Vertex normal
   GLint m_vertexColorLoc;             // Vertex color attribute location

   // Uniform locations
   GLint m_orthoMatrixLoc;             // Orthographic projection location (2-D)
   GLint m_pvmLoc;                     // Composite project, view, model matrix location
   GLint m_modelMatrixLoc;             // Model matrix location
   GLint m_normalMatrixLoc;            // Normal matrix location
   GLint m_modelViewMatrixLoc;         // Composite ModelView (V*M) matrix location
   GLint m_colorLoc;                   // Constant color
   GLint m_widthLoc;                   // Line width location
   GLint m_pointSizeLoc;               // Point size
   GLint m_cameraPositionLoc;          // Camera position loc
   GLint m_textureLoc;				   // texture location
   GLint m_vTexCoord;				   // vertex texture coordinate location

   // Material uniforms
   GLint m_materialAmbientLoc;
   GLint m_materialDiffuseLoc;
   GLint m_materialSpecularLoc;
   GLint m_materialEmissionLoc;
   GLint m_materialShininessLoc;

   // Lights
   int    m_maxEnabledLight;
   GLint  m_numLightsLoc;
   LightUniforms lights[2];

   // Current matrices
   float m_ortho[16];                  // Orthographic projection matrix (2-D)  (for use in GetStarted)
   Matrix4x4 m_projection;             // Current projection matrix
   Matrix4x4 m_view;                   // Current view matrix
   Matrix4x4 m_pvMatrix;               // Current composite projection and view matrix
   Matrix4x4 m_modelMatrix;            // Current model matrix

   // Retained state to push/pop modeling matrix
   std::list<Matrix4x4> m_modelMatrixStack;

   /**
    * Scene state constructor. Sets default values.
    */
   SceneState()
   {
      m_modelMatrixLoc = -1;
      m_modelViewMatrixLoc = -1;
      Init();
   }

   /**
    * Initialize scene state prior to drawing.
    */
   void Init() 
   {
      m_modelMatrix.SetIdentity();
      m_modelMatrixStack.clear();
   }

   /**
    * Copy current matrix onto stack
    */
   void PushTransforms()
   {
      m_modelMatrixStack.push_back(m_modelMatrix);
   }

   /**
    * Remove the current matrix from the stack and revert to prior
    * (or 0 if none are set at this node)
    */
   void PopTransforms()
   {  
      // If there are any matrices on the stack, retrieve the last one and remove it from the stack
      if (m_modelMatrixStack.size() > 0)
      {
         m_modelMatrix = m_modelMatrixStack.back();
         m_modelMatrixStack.pop_back();
      }
      else
         m_modelMatrix.SetIdentity();  
   }
};

#endif