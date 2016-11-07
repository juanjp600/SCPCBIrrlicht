// Copyright (C) 2015 Christian Holz
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__
#define __I_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__

#include "IMeshSceneNode.h"

namespace irr
{
namespace scene
{
	class ISceneManager;

	//! Scene node interface.
	/** A scene node is a node in the hierarchical scene graph. Every scene
	node may have children, which are also scene nodes. Children move
	relative to their parent's position. If the parent of a node is not
	visible, its children won't be visible either. In this way, it is for
	example easily possible to attach a light to a moving car, or to place
	a walking character on a moving platform on a moving ship.
	*/
	class IInstancedMeshSceneNode : public IMeshSceneNode
	{
	public:

		//! Constructor
		IInstancedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id = -1,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
				: IMeshSceneNode(parent, mgr, id, position, rotation, scale)
		{
		}


		//! Destructor
		virtual ~IInstancedMeshSceneNode()
		{
		}

		//! Adds a new instance as child of this node.
		/** Position, rotation, scale and id are taken from the passed node
		\param node: scene node of which the values are taken
		\return Pointer to the created empty scene node that represents
		the instance. This pointer should not be dropped. 
		See IReferenceCounted::drop() for more information. */
		virtual ISceneNode* addInstance(ISceneNode* node) = 0;

		//! Adds a new instance as child of this node.
		/** Position, rotation, scale and id are passed
		\param id: Id of the node. This id can be used to identify the scene node.
		\param position: Position of the space relative to its parent
		where the scene node will be placed.
		\param rotation: Initital rotation of the scene node.
		\param scale: Initial scale of the scene node.
		\return Pointer to the created empty scene node that represents
		the instance. This pointer should not be dropped.
		See IReferenceCounted::drop() for more information. */
		virtual ISceneNode* addInstance(const core::vector3df& position,
			const core::vector3df& rotation = core::vector3df(0, 0, 0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			s32 id = -1) = 0;

		//! Removes an instance from the list by index.
		/** \param index: index of the node that will be deleted
		\return True if the node was found and deleted. */
		virtual bool removeInstance(u32 index) = 0;

		//! Removes an instance from the list by id of the node.
		/** \param id: id of the node that will be deleted
		\return True if the node was found and deleted. */
		virtual bool removeInstanceById(s32 id) = 0;

		//! Removes an instance from the list by a node.
		/** \param node: node that will be deleted
		\return True if the node was found and deleted. */
		virtual bool removeInstance(ISceneNode* node) = 0;

		//! Returns a node in the instance list by index.
		/** \param index: index of the node in the list
		\return Node if it was found. */
		virtual ISceneNode* getInstance(u32 index) const = 0;

		//! Returns a node in the instance list by id of an node.
		/** \param id: node id that will be searched for
		\return Node if it could be found. */
		virtual ISceneNode* getInstanceById(s32 id) const = 0;

		//! Returns the instance count.
		/** \return Number of all instances. */
		virtual u32 getInstanceCount() const = 0;

		//! Updates the instance data that will pushed to the gpu
		virtual void updateInstances() = 0;

		//! Returns if the instanceScenenode is static or not
		/** \return True if it is static */
		virtual bool isStatic() const = 0;

		//! Sets the instancescenenode to static or not
		/** If the instancescenenode is static no instance
		data is updated every frame. Instead of updateInstances()
		has to be called manually.
		*/
		virtual void setStatic(bool staticInstances = true) = 0;

	protected:
	};


} // end namespace scene
} // end namespace irr

#endif