//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    TransformNode.h
//	Purpose:	Scene graph transformation node.
//
//============================================================================

#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

#include <vector>
#include "geometry/geometry.h"

/**
 * Transform node. Applies a transformation. This class allows OpenGL style 
 * transforms applied to the scene graph.
 */
class TransformNode: public SceneNode
{
public:
   /**
    * Constructor. Sets the node type to TRANSFORM and loads the identity matrix.
    */
	TransformNode()
   { 
      m_nodeType = SCENE_TRANSFORM; 
      LoadIdentity();
   }

   /**
    * Destructor.
    */
	virtual ~TransformNode() { }

   /**
	 * Set the stored matrix to the identity matrix.
	 */
	void LoadIdentity()
	{
		m_matrix.SetIdentity();
	}
	
	/**
	 * Apply a translation to the stored matrix.
    * @param  x   x translation
    * @param  y   y translation
    * @param  z   z translation
	 */
	void Translate(const float x, const float y, const float z)
	{
		m_matrix.Translate(x, y, z);
	}
	
	/**
	 * Apply a rotation to the stored matrix.
    * @param  deg   Rotation angle in degrees
    * @param  x     x component of the roation axis
    * @param  y     y component of the roation axis
    * @param  z     z component of the roation axis
	 */
	void Rotate(const float deg, const float x, const float y, const float z)
	{
		m_matrix.Rotate(deg, x, y, z);
	}

	/**
	 * Apply a scale to the stored matrix.
    * @param  x   x scaling
    * @param  y   y scaling
    * @param  z   z scaling
	 */
	void Scale(const float x, const float y, const float z)
	{
		m_matrix.Scale(x, y, z);
	}

	/**
	 * Draw this transformation node and its children. Note how this uses push
    * and pop to retain state.
	 */
	virtual void Draw(SceneState& sceneState)
	{
      // Copy current transforms onto stack
      sceneState.PushTransforms();

      // Apply this modeling transform to the current modeling matrix. Note the postmultiply -
      // this allows hierarchical transformations in the scene
      sceneState.m_modelMatrix *= m_matrix;

      if (sceneState.m_modelMatrixLoc != -1)
      {
         // Set the model matrix in the shader. This is NOT used in Animation3D. 
         glUniformMatrix4fv(sceneState.m_modelMatrixLoc, 1, GL_FALSE, sceneState.m_modelMatrix.Get());

         // Set the normal transformation matrix
         Matrix4x4 normalMatrix = sceneState.m_modelMatrix.GetInverse().Transpose();
         glUniformMatrix4fv(sceneState.m_normalMatrixLoc, 1, GL_FALSE, normalMatrix.Get());
      }

      if (sceneState.m_modelViewMatrixLoc != -1)
      {
         // Set a model view composite matrix. Lighting is computed in view space
         // NOTE - this is used for Animation3D but not for LightingViewing
         Matrix4x4 mv = sceneState.m_view * sceneState.m_modelMatrix;
         glUniformMatrix4fv(sceneState.m_modelViewMatrixLoc, 1, GL_FALSE, mv.Get()); 

         // Set the normal transform matrix (transpose of the inverse of the modelview matrix).
         // This transforms normals into view coordinates
         Matrix4x4 normalMatrix = mv.GetInverse().Transpose();
         glUniformMatrix4fv(sceneState.m_normalMatrixLoc, 1, GL_FALSE, normalMatrix.Get());
      }

      // Set the composite projection, view, modeling matrix
      Matrix4x4 pvm = sceneState.m_pvMatrix * sceneState.m_modelMatrix;
      glUniformMatrix4fv(sceneState.m_pvmLoc, 1, GL_FALSE, pvm.Get());

      // Draw all children
		SceneNode::Draw(sceneState);

      // Pop matrix stack to revert to prior matrices
      sceneState.PopTransforms();
	}

   /**
	 * Update the scene node and its children
	 */
	virtual void Update(SceneState& sceneState)
   {
      SceneNode::Update(sceneState);
   }

protected:
   // Local modeling transformation
	Matrix4x4 m_matrix;
};

#endif