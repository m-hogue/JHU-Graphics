//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    ColorNode.h
//	Purpose:	Simple presentation node that defines a color.
//
//============================================================================

#ifndef __COLORNODE_H
#define __COLORNODE_H

#include <list>

/**
 * Color presentation node.
 */
class ColorNode: public PresentationNode
{
public:
   /**
    * Constructor.
    */
	ColorNode() { }

   /**
    * Constructor given a red, green, and blue value.
    */
   ColorNode(const float r, const float g, const float b)
   {
      materialColor.Set(r, g, b, 1.0f);
   }

   /**
    * Destructor
    */
	virtual ~ColorNode() { }

	/**
	 * Draw this presentation node and its children
	 */
	virtual void Draw(SceneState& sceneState)
	{
      // Set the current color and draw all children. Very simple lighting support
      glUniform3fv(sceneState.m_materialDiffuseLoc, 1, &materialColor.r);
      SceneNode::Draw(sceneState);
	}

protected:
   Color4 materialColor;
};

#endif