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

#ifndef __GEOMETRYNODE_H
#define __GEOMETRYNODE_H

#include <list>

/**
 * Geometry node base class. Stores and draws geometry.
 */
class GeometryNode: public SceneNode
{
public:
   /**
    * Constructor
    */
   GeometryNode() { }

   /**
    * Destructor
    */
   virtual ~GeometryNode() { }

   /**
    * Draw this geometry node. Geometry nodes are leaf nodes and have no children.
    * @param  sceneState  Current scene state
    */
   virtual void Draw(SceneState& sceneState)
   {
   }
};

#endif