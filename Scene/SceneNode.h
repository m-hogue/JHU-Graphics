//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    SceneNode.h
//	Purpose:	Scene graph node. All scene node classes inherit from this
//          base class.
//
//============================================================================

#ifndef __SCENENODE_H
#define __SCENENODE_H

#include <vector>

/**
 * Scene graph node: base class
 */
class SceneNode
{
public:
	/**
	 * Constructor. Set the reference count to 0.
	 */
   SceneNode() : m_referenceCount(0), m_nodeType(SCENE_BASE) { } 

	/**
	* Destructor
	 */
	virtual ~SceneNode() 
	{
		Destroy();
	}

	/**
	 * Release this object from memory
	 */
	void Release() 
	{
      // Decrement the reference count. Delete the object when reference 
      // count falls to 0
      m_referenceCount--;
      if (m_referenceCount <= 0)
		   delete this; 
	}
	
	/**
	 * Draw the scene node and its children. The base class just draws the
    * children. Derived classes can use this (SceneNode::Draw()) to draw
    * all children without having to duplicate this code.
    * @param  sceneState  Current scene state
	 */
	virtual void Draw(SceneState& sceneState)
	{
		// Loop through the list and draw the children
		std::vector<SceneNode*>::iterator i = m_children.begin();
		for ( ; i != m_children.end(); i++)
			(*i)->Draw(sceneState);
	}
	
	/**
	 * Update the scene node and its children
    * @param  sceneState  Current scene state
	 */
	virtual void Update(SceneState& sceneState)
	{
		// Loop through the list and update the children
		std::vector<SceneNode*>::iterator i = m_children.begin();
		for ( ; i != m_children.end(); i++)
			(*i)->Update(sceneState);
	}	
	
	/**
	 * Destroy all the children
	 */
	void Destroy()
	{
		for (std::vector<SceneNode*>::iterator i = m_children.begin();
					i != m_children.end(); i++)
			(*i)->Release();

      m_children.clear();
	}

	/**
	 * Add a child to this node. Increment the reference count of the child.
    * @param  node  Add a child node to this scene node.
	 */
	void AddChild(SceneNode* node)
	{
		m_children.push_back(node);
		node->m_referenceCount++;
	}

   /**
	 * Get the type of scene node
    * @return  Returns the type of hte scene node.
	 */
	SceneNodeType GetNodeType() const
	{
		return m_nodeType;
	}

   /**
    * Set the name for this scene node.
    * @param  name  Name for this scene node.
    */
	void SetName(const char* name)
	{
		m_name = name;
	}
	
   /**
    * Get the name for this scene node.
    * @return  Returns the name of this scene node.
    */
   const char* GetName() const
	{
		return m_name.c_str();
	}

protected:
	std::string             m_name;              
	SceneNodeType           m_nodeType;
	int                     m_referenceCount;
	std::vector<SceneNode*> m_children;
};

#endif