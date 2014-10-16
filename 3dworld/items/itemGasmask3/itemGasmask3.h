#ifndef ITEMGASMASK3_H_INCLUDED
#define ITEMGASMASK3_H_INCLUDED

#include "../items.h"

class itemGasmask3 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemGasmask3();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_GASMASK3;
        }

        bool updateItem();

        static itemGasmask3* createItemGasmask3();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMGASMASK3_H_INCLUDED
