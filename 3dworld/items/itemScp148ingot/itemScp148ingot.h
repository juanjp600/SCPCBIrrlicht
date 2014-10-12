#ifndef ITEMSCP148INGOT_H_INCLUDED
#define ITEMSCP148INGOT_H_INCLUDED

#include "../items.h"

class itemScp148ingot : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp148ingot();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP148INGOT;
        }

        bool updateItem();

        static itemScp148ingot* createItemScp148ingot();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP148INGOT_H_INCLUDED
