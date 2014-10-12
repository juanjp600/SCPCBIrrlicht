#ifndef ITEMSUPERGASMASK_H_INCLUDED
#define ITEMSUPERGASMASK_H_INCLUDED

#include "../items.h"

class itemSupergasmask : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemSupergasmask();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SUPERGASMASK;
        }

        bool updateItem();

        static itemSupergasmask* createItemSupergasmask();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSUPERGASMASK_H_INCLUDED
