#ifndef ITEMGASMASK_H_INCLUDED
#define ITEMGASMASK_H_INCLUDED

#include "../items.h"

class itemGasmask : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemGasmask();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_GASMASK;
        }

        bool updateItem();

        static itemGasmask* createItemGasmask();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMGASMASK_H_INCLUDED
